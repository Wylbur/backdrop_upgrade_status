<?php
/**
 * Hooks implemented on behalf of views module.
 */

/**
 * Implements hook_backdrop_upgrade_status_todo().
 */
function views_backdrop_upgrade_status_todo() {
  return array(
    'views' => array(
      'review' => 'views',
      'description' => 'Configuration must be in the database to convert automatically durring upgrade.',
    ),
  );
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter().
 */
function views_backdrop_upgrade_status_report($todo_status = FALSE) {
  // Get all the taxonomy terms in the system.
  $rows = array();
  $collapsed = TRUE;
  $views = views_get_all_views();
  foreach ($views as $view) {
    switch ($view->type) {
      case t('Normal'):
      case t('Overridden'):
        $type = t('In database');
        break;
      default:
        $type = t('In code');
        break;
    }
    $row = array(
      'data' => array(
        'name' => $view->human_name,
        'machine' => $view->name,
        'state' => $type,
        'description' => $view->description,
        'recommendation' => '',
      ),
    );

    if ($type == t('In code')) {
      $collapsed = FALSE;
      $row['class'] = array('error');
      $text = t('Edit and save this view.');
      $row['data']['recommendation'] = $text;
      if (user_access('administer views')) {
        $href = 'admin/structure/views/view/' . $view->name . '/edit';
        $options = array('query' => array('destination' => current_path()));
        $link = l($text, $href, $options);
        $row['data']['recommendation'] = $link;
      }
    }
    $rows[] = $row;
  }

  // Mark all views as reviewed when all are in the database.
  $todo_status = FALSE;
  if ($collapsed && !isset($renderable['intro']['backdrop_upgrade_status_todo']['#default_value']['views'])) {
    $todo_status = TRUE;
  }

  $return = array(
    'machine' => 'views',
    'title' => t('Views data'),
    'info' => t('All views should show STORAGE STATE as <strong>In database</strong> in order to be properly upgraded into Backdrop CMS. Note: You will first need to make the <em>Save</em> button appear on the Edit View form. Click on any setting to open a modal, then click <em>Cancel</em> to close the modal. The <em>Save</em> button will be at the top right.'),
    'header' => array(t('View name'), t('Machine name'), t('Storage state'), t('Description'), t('Recommendation')),
    'rows' => $rows,
    'empty' => '',
    'collapsed' => $collapsed,
    'todo_status' => $todo_status,
  );

  return $return;
}
