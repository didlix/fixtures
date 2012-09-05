<?php
  
 /**
  * Implements hook_fixtures_prepare_field
  * Prepare location fields
  *     
  * @param $data string of location i.e. GEOMETRYCOLLECTION(POINT(1.0626685 52.2972388))
  * @return array
  * @author Rachel Graves
  **/
function fixtures_fixtures_field_filefield_prepare($field_value, $field) {

  if('imagefield' === $field['widget']['module']) {
                                                   
    $prepared_field = array();                             
    $file = fixtures_remote_file_get_url($field_value);
                     
    if(is_object($file)) {
      $prepared_field[] = fixtures_prepare_cck_image_field($field, $file->filepath);   
    }                                                   
  } else {
    return drush_set_error('fixtures', 'fixtures_fixtures_field_filefield_prepare is only designed for imagefields as urls right now, no filefields.');
  }

  return $prepared_field;  
}            


/**      
 * Implementes hook_fixtures_field_FIELD_TYPE_template
 *
 * @return a formatted example for a fixture's template
 * @todo take note of allowed_values, allowed_values_php,
 *  prefix, suffix, required, default_value, default_value_php
 */
function fixtures_fixtures_field_filefield_template($field) {

  $template = array();
                        
  if('imagefield' === $field['widget']['module']) {    
    $template['type'] = 'imagefield';  
    $template['example'] = 'http://lorempixum.com/500/500/people/';      
  } else {
    $template['type'] = 'filefield';
    $template['example'] = '/path/to/file.pdf';    
  }
  
  return $template;
}


function fixtures_prepare_cck_image_field($field, $filepath) {
  $validators = array_merge(filefield_widget_upload_validators($field), imagefield_widget_upload_validators($field));
  $fieldFileDirectory = filefield_widget_file_path($field);  
  // This path does not necessarily exist already, so make sure it is available
  fixtures_remote_file_recursive_file_create_path($fieldFileDirectory);
  $file = field_file_save_file($filepath, $validators, $fieldFileDirectory);
  return $file;

}                  


/**
 * base path for the file location
 * 
 * @return
 *   path to file location
 */
function fixtures_remote_file_basepath() {
  $path='fixtures';
  $path = file_create_path($path);
  file_check_directory($path, FILE_CREATE_DIRECTORY);
  return $path;
}
         
/**
 * Writes the given data to the file system and drupals files table
 * 
 * Existing files are overriden.
 * 
 * @param array $info
 *   Info for the file to create
 * @param $temp
 *   Indicated whether the file should be temporary saved of permanent
 * 
 * @return
 *   File object or FALSE
 * 
 * @see remote_file_basepath()
 */
function fixtures_remote_file_write_file( $info, $temp=FALSE) {
  $exists = is_file($info['filepath']);
  if (!$exists) {
    file_save_data( $info['data'], $info['filepath'], FILE_EXISTS_REPLACE);
    global $user;
    $file = (object) array();
    $file->uid = $user->uid;
    $file->filename=$info['filename'];
    $file->filepath=$info['filepath'];
    $file->filemime=$info['filemime'];
    $file->filesize=$info['filesize'];
    if ($temp) {
      $file->status = FILE_STATUS_TEMPORARY;
    }
    else {
      $file->status = FILE_STATUS_PERMANENT;
    }
    $result=drupal_write_record('files', $file);
    if ($result!==FALSE) {
      return $file;
    }
    return FALSE;
  }
  else {
    $sql="SELECT * FROM {files} WHERE filepath='%s'";
    return db_fetch_object(db_query($sql, $info['filepath']));
  }
} 

/**
 * Gets the file given by the url
 * 
 * @param $url
 *   The given url
 * @return
 *   FALSE or file object for further processing
 */
function fixtures_remote_file_get_url($url, $temp=TRUE) {
  // using rand to ensure unique md5 as url is often the same as an image api is often used
  $md5_url=md5(rand() . $url); 
  $error = 'No error';
  static $urls;
  if (!isset($urls)) {
    $urls=array();
  }
  if (isset($urls[$md5_url])) {
    return $urls[$md5_url];
  }
  // TODO: Check for appropriate providers like wikipedia.org
  //$result=parse_url($url);
  //===
  //TODO: Only do the request if the file is not already on our server
  $result=drupal_http_request($url);
  if (isset($result->error)) {
    unset($result->data);
    $error=print_r($result, TRUE);
    return $error;
  }
  $info=array();
  $info['filemime']=$result->headers['Content-Type'];
  $info['filesize']=$result->headers['Content-Length'];
  $info['data']=$result->data;
  list($dummy, $ext)=explode('/', $info['filemime']);
  $file_name = $md5_url . '.' . $ext;
  $info['filepath']=fixtures_remote_file_basepath() . '/' . $file_name;
  $info['filename']=$file_name;             
  $file=fixtures_remote_file_write_file($info, $temp);
  if (!is_object($file)) {
    $error=print_r($info,TRUE);  
    return $error;
  }
  $urls[$md5_url]=$file;
  return $urls[$md5_url];
}   

/**
 * Create the path ourselves recursively
 */
function fixtures_remote_file_recursive_file_create_path($path) {
  if (!file_check_directory($path, FILE_CREATE_DIRECTORY)) {
    // Support DOS style
    $root=str_replace('\\','/',file_directory_path());
    $path=str_replace('\\','/',$path);
    // We only do our work from the file system root
    $roots=explode('/',$root);
    $paths=explode('/',$path);
    while (count($roots)) {
      array_shift($roots);
      array_shift($paths);
    }
    foreach ($paths as $dir) {
      $root.= '/' . $dir;
      file_check_directory($root, FILE_CREATE_DIRECTORY);
    }
  }
}

