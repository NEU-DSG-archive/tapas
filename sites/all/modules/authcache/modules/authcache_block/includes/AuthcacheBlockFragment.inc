<?php
/**
 * @file
 * Defines fragment for blocks.
 */

/**
 * Fragment handler for blocks.
 *
 * @see block.module
 */
class AuthcacheBlockFragment implements AuthcacheP13nFragmentInterface, AuthcacheP13nFragmentValidatorInterface {
  /**
   * Validate fragment arguments.
   */
  public function validate($block_ids) {
    include DRUPAL_ROOT . '/modules/block/block.module';

    $result = array();
    foreach ($block_ids as $block_id) {
      // Ensure that a block with this id is available. Note that drupal is not
      // yet fully bootstrapped here. Because block_load essentially wraps a
      // call to the database and does not trigger any hooks, it is acceptible
      // to call it already here.
      list($module, $delta) = explode('-', $block_id, 2);

      $result[$block_id] = block_load($module, $delta);
    }

    return $result;
  }

  /**
   * Render blocks.
   */
  public function render($block_id, $block, $context) {
    $build = _block_get_renderable_array(_block_render_blocks(array(
      $block_id => $block,
    )));

    return render($build);
  }
}
