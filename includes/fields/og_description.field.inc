<?php
  
 /**
  * Implements hook_fixtures_prepare_field
  * Prepare location fields
  *
  * @param $data string of location i.e. GEOMETRYCOLLECTION(POINT(1.0626685 52.2972388))
  * @return array
  * @author Rachel Graves
  **/
function fixtures_fixtures_field_og_description_prepare($field_value, $field) {
  return $field_value;  
}        

/**      
 * Implementes hook_fixtures_field_FIELD_TYPE_template
 *
 * @return Array
 *  
 * @todo take note of allowed_values, allowed_values_php,
 *  prefix, suffix, required, default_value, default_value_php
 */
function fixtures_fixtures_field_og_description_template($field) {
  $desc = array();
  $desc['type'] = $field['type'];
  return $desc;
}