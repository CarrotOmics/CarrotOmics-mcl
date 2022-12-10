<?php
/**
 * @file
 * Fix duplicated markers.
 *
 * doc : https://intranet.bioinfo.wsu.edu:4433/node/2112
 *
 *
 */
function mcl_fix_data_3($input_file = NULL) {

  $working_dir = dirname($input_file);
  $output_file = "$working_dir/fix_dup_marker." . date("Y-m-d-G-i-s") . ".sql";

  // Gets the variables.
  $cvterm_gm = MCL_CHADO_CVTERM::getCvterm('sequence', 'genetic_marker');
  if (!$cvterm_gm) {
    throw new Exception("genetic_marker\n");
  }
  $cvterm_id_gm = $cvterm_gm->getCvtermID();

  // Opens the files for reading and writing.
  $fdr = fopen($input_file, "r");
  $fdw = fopen($output_file, "w");
  $transaction = db_transaction();
  try {
    $markers        = array();
    while (!feof($fdr)) {
      $line = trim(fgets($fdr));
      if ($line == '') {
        continue;
      }
      $tmp            = explode("\t", $line);
      $feature_id     = trim($tmp[0]);
      $marker_name    = trim($tmp[1]);
      $genus          = trim($tmp[2]);
      $species        = trim($tmp[3]);
      $genus_c        = trim($tmp[4]);
      $species_c      = trim($tmp[5]);

      // Gets the corrent the marker.
      if ($genus == $genus_c && $species == $species_c) {
        $markers[$marker_name]['C'] = $feature_id;
      }
      else {
        if (!array_key_exists('W', $markers[$marker_name])) {
          $markers[$marker_name]['W'] = array($feature_id);
        }
        else {
          $markers[$marker_name]['W'][] = $feature_id;
        }
      }
    }
    fclose($fdr);
/*
    db_query("SET search_path TO chado, public;");

    // Updates markers in linker tables.
    $linker_tables = array (
      'feature_synonym',
      'analysisfeature',
      'feature_dbxref',
      'feature_stock',
      'feature_contact',
      'feature_pub',
      'featurepos',
      'featureloc',
      'feature_genotype',
      'feature_project',
    );
    foreach ($map_feature_id as $feature_id_w => $feature_id_r) {
      foreach ($linker_tables as $linker_table) {

        $primary_key = $linker_table . '_id';
        $sql = "SELECT $primary_key FROM chado.$linker_table WHERE feature_id = $feature_id_w";
        $result = db_query($sql);
        while ($id = $result->fetchField()) {

          $class_name = strtoupper('CHADO_' . $linker_table);
          $obj = $class_name::byKey(array($primary_key => $id));
          $arr = $obj->getMemberArr();
          unset($arr[$primary_key]);
          $arr['feature_id'] = $feature_id_r;
          $target = $class_name::byKey($arr);
          if (!$target) {
            $new_obj = new $class_name($arr);
            if (!$new_obj->insert()) {
              print "Failed to insert into $linker_table\n";
            }
            $list = $values = '';
            foreach ($arr as $k => $v) {
              $list   .= ",$k";
              $values .= ",'$v'";
            }
            $list = substr($list, 1);
            $values = substr($values, 1);
            $sql = "INSERT INTO $linker_table ($list) VALUES ($values);";
            fputs($fdw, "$sql\n");
          }
        }
      }
    }

    fclose($fdw);
    $transaction->rollback();
    */

  } catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    watchdog('mcl', $e->getMessage(), array(), WATCHDOG_ERROR);
    return array('flag' => FALSE);
  }
  print "\nDONE\n";
}

