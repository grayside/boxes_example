<?php

/**
 * Simple box to allow View selection, then render the first result of the View.
 * Links to the Views path will be auto-generated if the View is of the Page display
 * type if one is not manually configured.
 */
class boxes_example_boxes_views_simple extends boxes_box {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    return array(
      'view' => '',
      'more' => '',
      'use_view_title' => FALSE,
    );
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form() {
    $form = array();
    $form['use_view_title'] = array(
      '#title' => t('Default to View Title'),
      '#description' => t('If the title is left blank, generate a title from the selected View'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['use_view_title'],
    );
    $form['view'] = array(
      '#title' => t('Box view'),
      '#description' => t('Select a View to be featured in this box.'),
      '#type' => 'select',
      '#default_value' => $this->options['view'],
      '#options' => $this->views_options(),
    );
    $form['more'] = array(
      '#title' => t('Path to view more'),
      '#description' => t('Adds a "more" link to the specified path. If none is supplied and a "Page" View is selected, the View\'s own menu path will be used.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['more'],
    );
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $content = '';
    list ($view_name, $display_id) = explode(':', $this->options['view']);
    $view = views_get_view($view_name);
    $view->is_cacheable = FALSE;

    $view->display[$display_id]->display_options['items_per_page'] = 1;
    $view->display[$display_id]->display_options['defaults']['items_per_page'] = FALSE;
    $view->display[$display_id]->display_options['use_pager'] = 0;
    $view->display[$display_id]->display_options['defaults']['use_pager'] = FALSE;

    // Collect a single rendered row for display in the box.
    $content .= '<div class="box-view-' . $this->options['view'] . '">'
             . $view->execute_display($display_id) . '</div>';
    
    // Generate a "more" link based on box configuration or (for Page displays)
    // the selected View's path.
    if (empty($this->options['more']) && $view->display[$display_id]->display_plugin == 'page') {
      $more = $view->display[$display_id]->display_options['path'];
    }
    else {
      $more = $this->options['more'];
    }

    if ($more) {
      $content .= l(t('view more'), $more);
    }

    // Use a title if it was set, otherwise default to the View title.
    // If there is no Views title set, leave it as NULL--no title will be shown.
    $title = $this->get_box_title($display_id, $view, $this->title);

    return array(
      'delta' => $this->delta,
      'title' => $title,
      'subject' => $title,
      'content' => $content,
    );
  }

  function views_options() {
    static $list;
    if (empty($list)) {
      $views = views_get_all_views();
      ksort($views);
      foreach ($views as $view) {
        if (!isset($views[$view->name]->disabled) || !$views[$view->name]->disabled) {
          foreach ($view->display as $id => $display) {
            $display_id = $view->name . ':' . $id;
            $title = empty($display->title) ? $display_id : $display->options['title'];
            $list[$display_id] = $title;
          }
        }
      }
    }
    return $list;
  }

  /**
   * Identify the best human-readable name for the Views display.
   *
   * @param $display_id
   *  The Views display id for which the title is being generated.
   * @param $view
   *  The Views object in question.
   *
   * @return
   *  String. The best title available.
   */
  function get_box_title($display_id, $view, $title = NULL) {
    if ($title && isset($title)) {
      return check_plain($title);
    }
    if ($this->options['use_view_title']) {
      $view->set_display($display_id);
      $title = $view->get_title();
      if (empty($title)) {
        $title = str_replace('_', ' ', $view->name);
      }
      return $title;
    }
    return '';
  }
}