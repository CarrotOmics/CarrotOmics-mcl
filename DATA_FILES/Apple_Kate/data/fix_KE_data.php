<?php
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                     PARAMETERS                          #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                   GLOBAL VARIABLES                      #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                         MAIN                            #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
main();
exit;
#---------------------------------------------------------#
# main function
#---------------------------------------------------------#
function main () {

	if (!($fdr_stock = fopen('stock.txt', 'r'))) {
		die;
	}
	if (!($fdr_target = fopen('target.txt', 'r'))) {
		die;
	}
	if (!($fdw = fopen('output.txt', 'w'))) {
		die;
	}

	// reading infile
	$accessions = array();
	while (!feof($fdr_stock)) {
		$line= trim(fgets($fdr_stock));
		if ($line == '') {
		  continue;
		}
	  if (preg_match("/^#/", $line)) {
			fputs($fdw, "$line\n");
		  continue;
		}
		$tmp = explode("\t", $line);
    $acc    = trim($tmp[0]);
    $acc_lc = strtolower($acc);
    $accessions[$acc_lc] = $acc;
    
    if (isset($tmp[1])) {
      $aliases = $tmp[1];
      $tmp1 = explode(';', $aliases);
      foreach ((array)$tmp1 as $a) {
        $alias = trim($a);
        $alias_lc = strtolower($alias);
        $accessions[$alias_lc] = $acc;
      }
    }
	}
	
	while (!feof($fdr_target)) {
	  $line= trim(fgets($fdr_target));
		if ($line == '') {
		  continue;
		}
		if (preg_match("/^#/", $line)) {
			fputs($fdw, "$line\n");
		  continue;
		}
		$name = strtolower($line);
		if (!array_key_exists($name, $accessions)) {
		  print "Error : $name cannot be found\n";
		  exit;
		}
		fputs($fdw, "$name\t" . $accessions[$name] . "\n");
	}
	fclose($fdr_stock);
	fclose($fdr_target);
	fclose($fdw);
}
?>
