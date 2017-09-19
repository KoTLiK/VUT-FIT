<?php
#CHA xaugus09
iconv_set_encoding("internal_encoding", "UTF-8");

define("IS_OK", 0);
define("ERR_PARAM", 1);
define("ERR_INPUT", 2);
define("ERR_OUTPUT", 3);

define("CODE", 0);
define("SLASH", 1);
define("S_COM", 2);
define("M_COM", 3);
define("M_COM_2", 4);
define("STR", 5);

/**
 * Napoveda
 */
function help()
{
	echo "Usage of 'CHA' C Header Analysis:\n";
	echo "  --help                 Prints this help\n";
	echo "  --input=fileordir      Input file or directory for analysis\n";
	echo "  --output=filename      Output file\n";
	echo "  --pretty-xml=k         Defines XML tags indent ('k' optional, default k=4)\n";
	echo "  --no-inline            Skips inline functions\n";
	echo "  --max-par=n            Analysing functions with up to 'n' parameters\n";
	echo "  --no-duplicates        Skips rest functions with the same name\n";
	echo "  --remove-whitespace    Removes unnecessary whitespace\n";
}

/**
 * Kontrola hodnoty parametru, vyzaduje sa 'false' => prepinac bez hodnoty
 *     --help, --no-inline, --no-duplicates, --remove-whitespace
 *
 * @param      array   $arr    Pole parametrov
 * @param      string  $o      'Key' parametru
 */
function false_val_required(array $arr, $o)
{
	if ($arr[$o] !== false) {
		fwrite(STDERR, "Nespravne pouzitie parametru '--$o'!\n");
		exit(ERR_PARAM);
	}
}

/**
 * Odstrani prebytocne znaky z textu ako komentare, stringy, makra a bloky {...}
 *
 * @param      string  $text   Obsah suboru
 */
function filtering($text)
{
	$text = preg_replace('/\\\(\R)/u', '', $text); # odstrani [\+ENTER] (multi-line)
	$text = preg_split('//u', $text); # pole znakov
	$length = count($text);
	$i = 0;
	$curly = 0;
	$state = CODE;
	$result = "";

	while ($i < $length) {
		$c = $text[$i];
		switch ($state) {
			case CODE:
				if ($c === "/") $state = SLASH;
				else if ($c === '"' || $c === "'") {
					$str_end = $c;
					$state = STR;
					$result .= $c;
				} else $result .= $c;
				break;
			case SLASH:
				if ($c === "/") $state = S_COM;
				else if ($c === "*") $state = M_COM;
				else {
					$result .= $c;
					$state = CODE;
				}
				break;
			case S_COM:
				if ($c === "\n") $state = CODE;
				break;
			case M_COM:
				if ($c === "*") $state = M_COM_2;
				break;
			case M_COM_2:
				if ($c === "/") $state = CODE;
				else if ($c !== "*") $state = M_COM;
				break;
			case STR:
				if ($c === $str_end) {
					$state = CODE;
					$result .= $c;
				} else if ($c === "\\") $i++;
				break;
		}
		$i++;
	}
	$result = preg_replace('/\s*#.*/um', '', $result); # odstrani makra
	# odstrani priradenie kvoli moznym koliziam
	$result = preg_replace('/(?:\()?[^\(\;\}\{]*?\=[^\)\;]*(?:\))?/um', ';', $result);
	$result = trim($result);
	return $result;
}

$longopts = array(
	"help::", # Bez hodnoty!
	"input:",
	"output:",
	"pretty-xml::",
	"no-inline::", # Bez hodnoty!
	"max-par:",
	"no-duplicates::", # Bez hodnoty!
	"remove-whitespace::" # Bez hodnoty!
);
$options = getopt(NULL, $longopts);

if ($options === false) {
	fwrite(STDERR, "Chyba parametrov!\n");
	exit(ERR_PARAM);
} else foreach (array_keys($options) as $o)
	if (is_array($options[$o])) {
		# Pri viacnasobnom parametre sa vytvori pole konkretneho parametru
		fwrite(STDERR, "Viacnasobne parametre!\n");
		exit(ERR_PARAM);
	}

# *** Zakladne nastavenia skriptu ***
	$input = false;
	$attribute_dir = "./";
	$output_file = STDOUT;
	$pretty = false;
	$indent = 4; # zakladne nastavenie
	$no_inline = false;
	$max_par = false;
	$max_par_n = 0;
	$no_duplicates = false;
	$whitespace = false;

foreach (array_keys($options) as $o) {
	switch ($o) {
		case "help":
			false_val_required($options, $o);
			if ((count($options) === 1) && ($argc === count($options)+1)) { help(); exit(IS_OK); }
			fwrite(STDERR, "Parameter --help sa neda kombinovat s inymi parametrami!\n");
			exit(ERR_PARAM);
		case "input":
			$input = true;
			if (is_dir($options[$o])) { # Directory
				$attribute_dir = $options[$o];
				if ($attribute_dir[strlen($attribute_dir)-1] != "/") $attribute_dir .= "/";
				foreach (new RecursiveIteratorIterator(new RecursiveDirectoryIterator($attribute_dir)) as $filepath => $key)
					if (preg_match("/\.h$/", $filepath)) $files[] = $filepath; # vsetky subory *.h
			} else { # File
				$files[0] = $options[$o]; # jediny vstupny subor
				$attribute_dir = "";
			}
			break;
		case "output":
			$output = $options[$o];
			break;
		case "pretty-xml":
			$pretty = true;
			if ($options[$o] !== false) {
				if (is_numeric($options[$o]) && $options[$o] >= 0)
					$indent = $options[$o];
				else {
					fwrite(STDERR, "Nespravny format pre --$o\n");
					exit(ERR_PARAM);
				}
			}
			break;
		case "no-inline":
			false_val_required($options, $o);
			$no_inline = true;
			break;
		case "max-par":
			$max_par = true;
			if (is_numeric($options[$o]) && $options[$o] >= 0)
				$max_par_n = $options[$o];
			else {
				fwrite(STDERR, "Nespravny format pre --$o\n");
				exit(ERR_PARAM);
			}
			break;
		case "no-duplicates":
			false_val_required($options, $o);
			$no_duplicates = true;
			break;
		case "remove-whitespace":
			false_val_required($options, $o);
			$whitespace = true;
			break;
		default:
			fwrite(STDERR, "Chyba parametru!\n");
			exit(ERR_PARAM);
	}
}

if ($argc !== count($options)+1) { # kontrola poctu parametrov
	fwrite(STDERR, "Nespravne parametre! Viac o parametroch s '--help'.\n");
	exit(ERR_PARAM);
}

if (isset($output)) { # zmena vystupu ak bol zadany vystup
	@$output_file = fopen($output, 'w');
	if (!$output_file) {
		fwrite(STDERR, "Neda sa otvorit subor $output !\n");
		exit(ERR_OUTPUT);
	}
}

if (!$input) { # nezadany prepinac input
	foreach (new RecursiveIteratorIterator(new RecursiveDirectoryIterator($attribute_dir)) as $filepath => $key)
		if (preg_match("/\.h$/", $filepath)) $files[] = $filepath; # vsetky subory *.h
}

# *** Nastavenia XML outputu ***
	$xml = new XMLWriter;
	$xml->openMemory();
	$xml->startDocument('1.0', 'UTF-8');
	$xml->setIndent(true);
	if ($pretty) $xml->setIndentString(str_repeat(" ", $indent));
	else $xml->setIndentString("");
	$xml->startElement('functions');
	$xml->writeAttribute('dir', $attribute_dir);

$fce_pattern = '/\s*(?<rettype>(?:[a-zA-Z_][a-zA-Z_0-9]*[\s\*]+?)+)\s*(?<name>[a-zA-Z_][a-zA-Z_0-9]*)\s*\((?<args>[^\)\;]*)\)\s*(?:\;|\{)/u';

if (isset($files)) {
	foreach ($files as $file) { # Jednotlive subory
		@$text = file_get_contents($file);
		if ($text === false && $text !== "") {
			fclose($output_file);
			fwrite(STDERR, "Chyba pri otvarani suboru $file !\n");
			exit(ERR_INPUT);
		}
		$x_file = substr($file, strlen($attribute_dir)); # atribut FILE
		$text = filtering($text);

		$functions = array();
		$duplo = array();
		if (preg_match_all($fce_pattern, $text, $functions)) {
			for ($i = 0; $i < count($functions[0]); $i++) { # Jednotlive funkcie
				$x_rettype = preg_replace('/\s/u', " ", trim($functions['rettype'][$i])); # atribut RETTYPE
				$x_name = $functions['name'][$i]; # atribut NAME

				if ($no_inline && preg_match('/(^|\s+)inline\s/u', $x_rettype)) continue;

				$arguments = explode(',', $functions['args'][$i]);
				if (count($arguments) === 1) # Odstranenie argumentov bezparametrickych funkcii
					if (preg_match('/^\s*void\s*$|^\s*$/u', $arguments[0]))
						array_splice($arguments, 0);

				for ($a = 0; $a < count($arguments); $a++) # Uprava bielych znakov
					$arguments[$a] = preg_replace('/\s/u', " ", trim($arguments[$a]));

				if ($i_var = array_search("...", $arguments)) { # nastavenie atributu VARARGS
					array_splice($arguments, $i_var, 1);
					$x_varargs = "yes";
				} else $x_varargs = "no";

				if ($max_par && $max_par_n < count($arguments))
					continue; # Len vyvoleni mozu prejst

				if ($no_duplicates) {
					if (array_search($x_name, $duplo) !== false) continue;
					else $duplo[] = $x_name;
				}

				if ($whitespace) {
					$x_rettype = preg_replace('/\s+/u', " ", $x_rettype);
					$x_rettype = preg_replace('/\s*\*\s*/u', "*", $x_rettype);
					for ($a=0; $a < count($arguments); $a++) {
						$arguments[$a] = preg_replace('/\s+/u', " ", $arguments[$a]);
						$arguments[$a] = preg_replace('/\s*\*\s*/u', "*", $arguments[$a]);
					}
				}

				# *** Funkcia dotvori svoj tvar ***
				$xml->startElement('function');
					$xml->writeAttribute('file', $x_file);
					$xml->writeAttribute('name', $x_name);
					$xml->writeAttribute('varargs', $x_varargs);
					$xml->writeAttribute('rettype', $x_rettype);

				for ($a=0; $a < count($arguments); $a++) {
					preg_match_all('/(?<type>.*?)[a-zA-Z_][a-zA-Z_0-9]*\s*$/u', $arguments[$a], $arg_type);
					$x_type = trim($arg_type['type'][0]); # atribut TYPE
					$xml->startElement('param');
						$xml->writeAttribute('number', $a+1);
						$xml->writeAttribute('type', $x_type);
					$xml->endElement();
				}
				$xml->endElement();
			} # Jednotlive funkcie
		}
	} # Jednotlive subory
}

$xml->fullEndElement();
$xml->endDocument();
$xml_string = $xml->outputMemory(true);
if (!$pretty) $xml_string = str_replace("\n", "", $xml_string) ."\n";
fwrite($output_file, $xml_string);
fclose($output_file);
exit(IS_OK);
?>