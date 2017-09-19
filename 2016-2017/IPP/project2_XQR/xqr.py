#!/usr/local/bin/python3

# XQR: xaugus09

import sys
import argparse
import codecs
import re
import xml.dom.minidom


def error(msg, code):  # Vypise error hlasku a ukonci program s prislusnym kodom
    sys.stderr.write("ERROR: {}\n".format(msg))
    sys.exit(code)


def r_error(msg, code):  # Vyvola vynimku s formatom [sprava, navratovy kod]
    raise MyError({'msg': msg, 'code': code})


class ArgsError(Exception):
    pass


class MyError(Exception):
    pass


class AParser(argparse.ArgumentParser):
    def error(self, message):  # override pre vlastne error vypisy
        raise ArgsError


class Options:
    """Kontrola argumentov z prikazoveho riadku,
       nesie hodnoty nastavenia skriptu"""
    def __init__(self):  # inicializacia a pripadny vypis --help
        self.options = {
            'input': sys.stdin,
            'output': sys.stdout,
            'query': '',
            'no_head': False,
            'root': ''
        }

        if (len(sys.argv) == 2) and (sys.argv[1] == "--help"):
            sys.stdout.write("Pouzitie pre 'XQR' XML Query:\n"
                             "  --help             Vypise tuto napovedu.\n"
                             "  --input=filename   Vstupny subor vo formate XML (implicitne standartny vstup)\n"
                             "  --output=filename  Vystupny subor vo formate XML (implicitne standartny vystup)\n"
                             "  --query='dotaz'    Dotaz v dotazovacom jazyku (nie je mozna kombinacia s --qf)\n"
                             "  --qf=filename      Dotaz v dotazovacom jazyku v externom textovom subore\n"
                             "                     (nie je mozna kombinacia s --query)\n"
                             "   (povinny jeden z prepinacov [--query|--qf])\n"
                             "  -n                 Negenerovanie XML hlavicky na vystupe\n"
                             "  --root=element     Meno paroveho korenoveho elementu obalujuci vysledky\n")
            sys.exit(0)

    def fclose(self):
        self.options['input'].close()
        self.options['output'].close()

    def parse_args(self):  # kontrola parametrov a ich pripadna uprava
        parser = AParser(add_help=False)
        parser.add_argument('--help', action='store_true')
        parser.add_argument('--input')
        parser.add_argument('--output')
        parser.add_argument('--query')
        parser.add_argument('--qf')
        parser.add_argument('-n', action='store_true')
        parser.add_argument('--root')

        args = parser.parse_args()  # raise ArgsError

        # ------------------------ Kontrola vsetkych parametrov ------------------------
        if args.help:
            r_error('Parameter --help sa neda kombinovat s inymi parametrami!', 1)

        if args.query and args.qf:
            r_error('Musi byt pouzity prave jeden z prepinacov [--query, --qf]!', 1)

        if args.query is None and args.qf is None:
            r_error('Musi byt pouzity prave jeden z prepinacov [--query, --qf]!', 80)

        if args.input is not None:
            if args.input == '':
                r_error('Nezadany subor [input]!', 1)
            else:
                try:
                    self.options['input'] = codecs.open(args.input, 'r', 'utf-8')
                except:
                    r_error('Neuspesne otvaranie vstupneho suboru', 2)

        if args.output is not None:
            if args.output == '':
                r_error('Nezadany subor [output]!', 1)
            else:
                try:
                    self.options['output'] = open(args.output, 'w')
                except:
                    r_error('Neuspesne otvaranie vystupneho suboru', 3)

        if args.qf is not None:
            if args.qf == '':
                r_error('Nezadany subor [qf]!', 80)  # TODO Skontrolovat
            else:
                try:
                    f = open(args.qf, 'r')
                    self.options['query'] = f.read()
                    f.close()
                except:
                    r_error('Neuspesne otvaranie vstupneho suboru', 80)
        else:  # zabezpecene, ze query existuje v podmienke vyssie, ak qf nie
            if args.query == '':
                r_error('Nezadany dotaz [query]!', 80)
            else:
                self.options['query'] = args.query

        if args.root is not None:
            if args.root == '':
                r_error('Nezadany root element!', 1)
            else:
                self.options['root'] = args.root

        self.options['no_head'] = args.n

    def get_options(self):
        return self.options


class Query:
    """Skontroluje dotaz query, ci je validny.
       Vyuziva sa rekurzivny zostup, entry point je metoda _query, vyvolana metodou check_query"""
    def __init__(self):
        self.kw = ('SELECT', 'FROM', 'ROOT', 'WHERE', 'NOT', 'CONTAINS', 'LIMIT')
        self.op = ('=', '<', '>')
        self.query = ''
        self.token = ''
        self.temp = ''
        self.table = dict()
        self.rx = re.compile(r"^[a-zA-Z_][\w]*$")

    def check_query(self, query):
        """Kontrola dotazu"""
        for i in self.op:
            rx = re.compile(r"{}".format(i))
            if rx.search(query) is not None:  # operatory [<, >, =] obalim o 1 SPACE z kazdej strany
                query = rx.sub(" {} ".format(i), query)

        rx = re.compile(r"^\'.+\'$")  # preventivne aj stringy '...'
        match = rx.search(query)
        if match is not None:
            query = rx.sub(" {} ".format(match.group(0)), query)
        else:
            rx = re.compile(r"^\".+\"$")  # preventivne aj stringy "..."
            match = rx.search(query)
            if match is not None:
                query = rx.sub(" {} ".format(match.group(0)), query)

        rx = re.compile(r"\s+")  # vsetky skupiny bielych znakov nahradim za 1 space
        self.query = rx.sub(' ', query).strip() + ' '
        self.temp = self.query

        if not self._query():
            r_error('Nespravny format dotazu!', 80)

    def next_token(self):
        pos = self.temp.find(' ')
        self.token = self.temp[:pos]
        self.token = self.token.strip()
        self.temp = self.temp[len(self.token) + 1:]
        if self.token == '':
            r_error('Nespravny format dotazu!', 80)

    def __word(self, word):
        """Kontrola ci ide o platne slovo, mimo skupinu KEY WORDS"""
        match = self.rx.search(word)
        if match is None:
            return False
        for i in self.kw:
            if match.group(0).find(i) is not -1:
                return False
        return True

    def __el_at(self, word):
        """Vrati list [word , element, attribute]"""
        pos = word.find('.')
        if pos is -1:
            if not self.__word(word):
                return None, None, None
            return word, word, None  # samotny element
        elif len(word) is not pos + 1:
            if pos:
                el = word[:pos]  # element
                if not self.__word(el):
                    return None, None, None

                at = word[pos + 1:]  # .attribute (bez bodky)
                if not self.__word(at):
                    return None, None, None
                return word, el, at
            else:
                at = word[pos + 1:]  # samotny .attribute (bez bodky)
                if not self.__word(at):
                    return None, None, None
                return word, None, at
        return None, None, None

    def _query(self):
        """<ORDER-BY> je preskocene pretoze rozsirenie nie je v umysle"""
        self.next_token()
        if self.token != "SELECT":  # SELECT
            return False

        self.next_token()
        if not self.__word(self.token):  # element
            return False
        self.table['select'] = self.token

        self.next_token()
        if self.token != "FROM":  # FROM
            return False

        try:
            self.next_token()
        except MyError:
            self.table['from'] = ''
            self.table['where'] = False
            self.table['limit'] = None
            return True

        if not self._from_elem():  # <FROM-ELM>
            return False

        if self.table['from'] != '':
            try:
                self.next_token()
            except MyError:
                self.table['where'] = False
                self.table['limit'] = None
                return True

        if not self._where_clause():  # <WHERE-CLAUSE>
            return False

        if self.table['where']:
            try:
                self.next_token()
            except MyError:
                self.table['limit'] = None
                return True

        if not self._limit():  # <LIMITn>
            return False

        try:
            self.next_token()
        except MyError:
            return True

        return False

    def _from_elem(self):
        if self.token == "WHERE" or self.token == "LIMIT":
            self.table['from'] = ''
            return True
        elif self.token == "ROOT":
            self.table['from'] = "ROOT"
            return True

        self.table['from'], self.table['from_e'], self.table['from_a'] = self.__el_at(self.token)
        if self.table['from'] is not None:
            return True

        return False

    def __not(self):
        """Vyhodnoti ci sa podmienka ma/nema negovat"""
        self.table['c_not'] = False
        while True:
            self.next_token()
            if self.token != "NOT":
                break
            else:
                self.table['c_not'] = not self.table['c_not']

    def __literal_str(self, word):
        """Kontrola ci ide o string ohraniceny \' alebo \" """
        regex1 = re.compile(r"^\'.+\'$")
        regex2 = re.compile(r"^\".+\"$")
        if (regex1.search(word) is not None) or (regex2.search(word) is not None):
            return True
        return False

    def __literal_int(self, signed=True):
        """Skontroluje ci literal je formatu [+/-]INT
           flag [signed] urcije ci kontroluje [+/-]INT alebo iba [+]INT"""
        if signed:
            regex = re.compile(r"^[\+\-]?\d+$")
            if regex.search(self.token) is None:
                return False
        else:
            regex = re.compile(r"^\+?\d+$")
            if regex.search(self.token) is None:
                return False
        return True

    def _where_clause(self):
        if self.token != "WHERE":  # WHERE
            if self.token == "LIMIT":
                self.table['where'] = False
                return True
            return False
        self.table['where'] = True

        self.__not()  # <condition> NOT

        self.table['c1'], self.table['c1_e'], self.table['c1_a'] = self.__el_at(self.token)
        if self.table['c1'] is None:  # <condition> element/attribute
            return False

        self.next_token()  # <condition> operator
        if self.token in self.op:
            self.table['c_op'] = self.token
        elif self.token == "CONTAINS":
            self.table['c_op'] = "C"
        else:
            return False

        self.next_token()  # <condition> literal
        self.table['c2_str'] = None
        self.table['c2_int'] = None
        if self.table['c_op'] == "C":
            if not self.__literal_str(self.token):  # contains vyzaduje string
                return False
            self.table['c2'] = self.token
            self.table['c2_str'] = self.token.strip('\'\"')
        else:
            if self.__literal_str(self.token):
                self.table['c2'] = self.token
                self.table['c2_str'] = self.token.strip('\'\"')
            elif self.__literal_int():
                self.table['c2'] = self.token
                self.table['c2_int'] = int(self.token)
            else:
                return False

        return True

    def _limit(self):
        if self.token != "LIMIT":  # LIMIT
            return False

        self.next_token()
        if not self.__literal_int(False):
            return False
        self.table['limit'] = int(self.token)
        return True

    def get_query_table(self):
        return self.table


class Selection:
    def __init__(self, input_doc, query):
        """Vyselektuje elementy podla dotazu 'query' zo suboru/vstupu 'input_doc' """
        try:
            self.dom = xml.dom.minidom.parse(input_doc)
        except:
            r_error('Chybny format vstupneho suboru', 4)
        self.query = query
        self.elements = []
        self.limit = None if query['limit'] is None else int(query['limit'])
        self.root = None
        self.true = []
        self.false = []

        # ############## FROM + SELECT ##############
        # Do 'self.elements' sa ulozia vsetky elementy (SELECT element) z root elementu (FROM <from-elem>)
        if query['from'] == '':  # SKIP
            return
        elif query['from'] == "ROOT":  # ROOT
            self.elements.extend(self.dom.getElementsByTagName(query['select']))
        elif query['from_e'] is None:  # .attribute
            for elem in self.dom.getElementsByTagName("*"):
                if elem.hasAttribute(query['from_a']):
                    self.root = elem
                    break
            if self.root is None:
                return
            self.elements.extend(self.root.getElementsByTagName(query['select']))
        elif query['from_a'] is None:  # element
            for elem in self.dom.getElementsByTagName(query['from_e']):
                self.root = elem
                break
            if self.root is None:
                return
            self.elements.extend(self.root.getElementsByTagName(query['select']))
        else:  # element.attribute
            for elem in self.dom.getElementsByTagName(query['from_e']):
                if elem.hasAttribute(query['from_a']):
                    self.root = elem
                    break
            if self.root is None:
                return
            self.elements.extend(self.root.getElementsByTagName(query['select']))

        # ############## filter WHERE + condition ##############
        if query['where']:
            # Vyfiltrovanie elementov podla klauzuli WHERE z 'self.elements'
            # Vyhodnotenie podmienky a rozdelenie elementov do TRUE a FALSE vetvy (listov)
            self._where_filter(self.elements, query['c1_e'], query['c1_a'])
            # Kontrola ci sa ma vykonat negacia
            if query['c_not']:  # True -> Znegujeme
                self.elements = self.false
            else:  # False -> Nenegujeme
                self.elements = self.true

    def _where_filter(self, elements, query_e, query_a):
        """Vyfiltruje elementy, ktore budu vstupovat do podmienky v klauzuli WHERE
           elements -> hruby select elementov
           query_e -> element
           query_a -> .attribute"""

        if query_a is None:  # element
            for i in elements:
                if i.tagName == query_e:  # aktualny element
                    if self._where_condition(i):
                        self.true.append(i)
                    else:
                        self.false.append(i)
                else:  # prehladanie do hlbky
                    self.root = i.getElementsByTagName(query_e)
                    if len(self.root):
                        if self._where_condition(self.root[0]):  # vyhodnotenie podmienky
                            self.true.append(i)
                        else:
                            self.false.append(i)
                    else:
                        self.false.append(i)  # Nenasiel sa vyskyt -> false
        elif query_e is None:  # .attribute
            for i in elements:
                if i.hasAttribute(query_a):  # aktualny element
                    if self._where_condition(i):
                        self.true.append(i)
                    else:
                        self.false.append(i)
                else:  # prehladanie do hlbky
                    self.root = None
                    for elem in i.getElementsByTagName("*"):
                        if elem.hasAttribute(query_a):
                            self.root = elem
                            break
                    if self.root is not None:
                        if self._where_condition(self.root):  # vyhodnotenie podmienky
                            self.true.append(i)
                        else:
                            self.false.append(i)
                    else:
                        self.false.append(i)  # Nenasiel sa vyskyt -> false
        else:  # element.attribute
            for i in elements:
                if i.tagName == query_e and i.hasAttribute(query_a):  # aktualny element
                    if self._where_condition(i):
                        self.true.append(i)
                    else:
                        self.false.append(i)
                else:  # prehladanie do hlbky
                    self.root = None
                    for elem in i.getElementsByTagName(query_e):
                        if elem.hasAttribute(query_a):
                            self.root = elem
                            break
                    if self.root is not None:
                        if self._where_condition(self.root):  # vyhodnotenie podmienky
                            self.true.append(i)
                        else:
                            self.false.append(i)
                    else:
                        self.false.append(i)  # Nenasiel sa vyskyt -> false

    def _where_condition(self, element):
        """Vyhodnoti <condition> nad vybranym 'element'-om: return True/False """
        if self.query['c1_a'] is None:  # element

            if self.query['c2_int'] is None:  # element <operator> string
                if len(element.childNodes) == 1 and type(element.firstChild) == xml.dom.minidom.Text:
                    if self.query['c_op'] == "C":  # CONTAINS
                        return self.query['c2_str'] in element.firstChild.nodeValue
                    elif self.query['c_op'] == "<":  # '<'
                        return element.firstChild.nodeValue < self.query['c2_str']
                    elif self.query['c_op'] == ">":  # '>'
                        return element.firstChild.nodeValue > self.query['c2_str']
                    else:  # '='
                        return element.firstChild.nodeValue == self.query['c2_str']
                else:
                    r_error('Chybny format vstupneho suboru', 4)

            else:  # element <operator> integer
                try:
                    if len(element.childNodes) == 1 and type(element.firstChild) == xml.dom.minidom.Text:
                        if self.query['c_op'] == "<":  # '<'
                            return float(element.firstChild.nodeValue) < self.query['c2_int']
                        elif self.query['c_op'] == ">":  # '>'
                            return float(element.firstChild.nodeValue) > self.query['c2_int']
                        else:  # '='
                            return float(element.firstChild.nodeValue) == self.query['c2_int']
                    else:
                        r_error('Chybny format vstupneho suboru', 4)
                except ValueError:  # float conversion
                    return False

        else:  # element.attribute / .attribute

            if self.query['c2_int'] is None:  # element.attribute / .attribute <operator> string
                if self.query['c_op'] == "C":  # CONTAINS
                    return self.query['c2_str'] in element.getAttribute(self.query['c1_a'])
                elif self.query['c_op'] == "<":  # '<'
                    return element.getAttribute(self.query['c1_a']) < self.query['c2_str']
                elif self.query['c_op'] == ">":  # '>'
                    return element.getAttribute(self.query['c1_a']) > self.query['c2_str']
                else:  # '='
                    return element.getAttribute(self.query['c1_a']) == self.query['c2_str']

            else:  # element.attribute / .attribute <operator> integer
                try:
                    if self.query['c_op'] == "<":  # '<'
                        return float(element.getAttribute(self.query['c1_a'])) < self.query['c2_int']
                    elif self.query['c_op'] == ">":  # '>'
                        return float(element.getAttribute(self.query['c1_a'])) > self.query['c2_int']
                    else:  # '='
                        return float(element.getAttribute(self.query['c1_a'])) == self.query['c2_int']
                except ValueError:  # float conversion
                    return False

    def get_elements(self):
        """Vrati list elementov, pripadne orezanych klauzulou LIMIT"""
        if self.limit is None:
            return self.elements
        else:
            return self.elements[:self.limit]


if __name__ == '__main__':
    options = Options()
    try:
        options.parse_args()
        o = options.get_options()

        sql_query = Query()
        sql_query.check_query(o['query'])

        select = Selection(o['input'], sql_query.get_query_table()).get_elements()

        if not o['no_head']:
            o['output'].write('<?xml version="1.0" encoding="UTF-8"?>')

        if len(select):
            if o['root'] != '':
                o['output'].write('<'+o['root']+'>')
            for element in select:
                o['output'].write(element.toxml())
            if o['root'] != '':
                o['output'].write('</'+o['root']+'>')
        else:
            if o['root'] != '':
                o['output'].write('<'+o['root']+'/>')

        o['output'].write('\n')

    except ArgsError:
        error("Nespravne parametre!", 1)
    except MyError as err:
        options.fclose()
        error(err.args[0]['msg'], err.args[0]['code'])

    options.fclose()
    sys.exit(0)  # Vsetko ok, hotovo
