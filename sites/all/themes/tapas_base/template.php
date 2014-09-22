<?php
/*
 * Implements hook_preprocess_html()
 * Set up variables from the environment
 *
 *
 */
 //REMVOVED and moved to css injector. this seems overly complex just to change the menu item background! saved for reference
 /*
 //ADDED This annoying function is here to set the active state for dynamic user menus so that they can look pretty. whatever
function tapas_base_menu_link(array $variables) {
		foreach($variables as &$user_menu) {
			//I have no idea why these get removed simply by running this function...but here they are back. grr
			$user_menu['#attributes']['class'][]='menu-item-'.$user_menu['#original_link']['mlid'];
			$user_menu['#localized_options']['attributes']['class'][]='menu-item-'.$user_menu['#original_link']['mlid'];
			$user_menu['#original_link']['localized_options']['attributes']['class'][] ='menu-item-'.$user_menu['#original_link']['mlid'];
			
			if( $user_menu['#theme'] == 'menu_link__user_menu' && $user_menu['#title'] == 'My Account'){
//echo "<pre>";
//print_r($user_menu);
//echo "</pre>BREAK";
				$the_url = request_uri();
				$get_url=ltrim($the_url, '/');
				//echo $get_url;

				global $user;
				$uid=$user->uid;
				$ulink = drupal_get_path_alias('user/' . $uid);
				//echo $ulink;

				if($ulink==$get_url){
					$user_menu['#attributes']['class'][]='active-trail';
					$user_menu['#localized_options']['attributes']['class'][] = 'active-trail';
					$user_menu['#localized_options']['attributes']['class'][]='active';
					$user_menu['#original_link']['localized_options']['attributes']['class'][] = 'active-trail';
					$user_menu['#original_link']['localized_options']['attributes']['class'][]='active';
					$user_menu['#original_link']['in_active_trail']=TRUE;
//echo "<pre>";
//print_r($user_menu);
//echo "</pre>";
				}		
			}
		}
		return 
		theme_menu_link($variables);
}



//ADDED I THINK WE CAN PROBABLY DELETE THIS search results customization based on http://drupal.org/node/175013
/*
function tapas_base_search_item($item, $type) {
  $output = ' <dt class="title"><a href="'. check_url($item['link']) .'">'. check_plain($item['title']) .'where is the change?</a></dt>';
  $output .= ' <dd>'. ($item['snippet'] ? '<p>'. $item['snippet'] .'</p>' : '');
  return $output;
}
/*
function tapas_base_search_item($item, $type) {
  $output = ' <dt class="title"><a href="'. check_url($item['link']) .'">'. check_plain($item['field_title']) .'</a></dt>';
  $info = array();
  if ($item['type']) {
    $info[] = check_plain($item['type']);
  }
  if ($item['user']) {
    $info[] = $item['user'];
  }
  if ($item['date']) {
    $info[] = format_date($item['date'], 'small');
  }
  if (is_array($item['extra'])) {
    $info = array_merge($info, $item['extra']);
  }
  $output .= ' <dd>'. ($item['snippet'] ? '<p>'. $item['snippet'] .'</p>' : '') .'<p class="search-info">'. implode(' - ', $info) .'</p></dd>';
  return $output;
}
*/
//END
//ADDED method from http://www.alisonhover.com/blog/201208/how-customise-default-file-icons-drupal-7 to have custom file icons
function tapas_base_file_icon($variables) {
  $file = $variables['file'];
  $icon_directory = drupal_get_path('theme', 'tapas_base') . '/images/icons';
//exit(print_r($icon_directory));
  $mime = check_plain($file->filemime);
  $icon_url = file_icon_url($file, $icon_directory);
  return '<img alt="" class="file-icon" src="' . $icon_url . '" title="' . $mime . '" />';
}
//END

// Pixture Reloaded by Adaptivethemes.com
/**
 * Override or insert variables into the html template.
 */
function tapas_base_preprocess_html(&$vars) {
  global $theme_key;

  $theme_name = 'tapas_base';
  $path_to_theme = drupal_get_path('theme', $theme_name);

  // Add a class for the active color scheme
  if (module_exists('color')) {
    $class = check_plain(get_color_scheme_name($theme_key));
    $vars['classes_array'][] = 'color-scheme-' . drupal_html_class($class);
  }

  // Add class for the active theme
  $vars['classes_array'][] = drupal_html_class($theme_key);

  // Add theme settings classes
  $settings_array = array(
    'box_shadows',
    'body_background',
    'menu_bullets',
    'menu_bar_position',
    'corner_radius',
  );
  foreach ($settings_array as $setting) {
    $vars['classes_array'][] = theme_get_setting($setting);
  }

  // Special case for PIE htc rounded corners, not all themes include this
  if (theme_get_setting('ie_corners') == 1) {
    drupal_add_css($path_to_theme . '/css/ie-htc.css', array(
      'group' => CSS_THEME,
      'browsers' => array(
        'IE' => 'lte IE 8',
        '!IE' => FALSE,
        ),
      'preprocess' => FALSE,
      )
    );
  }
}

/**
 * Override or insert variables into the html template.
 */
function tapas_base_process_html(&$vars) {
  // Hook into color.module
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

/**
 * Override or insert variables into the page template.
 */
function tapas_base_process_page(&$vars) {
  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}

/**
 * Override or insert variables into the block template.
 */
function tapas_base_preprocess_block(&$vars) {
  if($vars['block']->module == 'superfish' || $vars['block']->module == 'nice_menu') {
    $vars['content_attributes_array']['class'][] = 'clearfix';
  }
}


// Add some cool text to the search block form
function tapas_base_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    // HTML5 placeholder attribute
    $form['search_block_form']['#attributes']['placeholder'] = t('Search TAPAS');
  }
}