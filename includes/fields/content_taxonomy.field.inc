<?php
  
 /**
  * Implements hook_fixtures_prepare_field
  * Prepare location fields
  *
  * @param $data string of location i.e. GEOMETRYCOLLECTION(POINT(1.0626685 52.2972388))
  * @return array
  * @author Rachel Graves
  **/
function fixtures_fixtures_field_content_taxonomy_prepare($field_value, $field) {
  $prepared_field = array();
  $prepared_field[] = $field_value;     
  
  return $prepared_field;  
}