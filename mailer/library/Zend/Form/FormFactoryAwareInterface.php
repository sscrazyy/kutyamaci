<?php
/**
 * Zend Framework (http://framework.zend.com/)
 *
 * @link      http://github.com/zendframework/zf2 for the canonical source repository
 * @copyright Copyright (c) 2005-2012 Zend Technologies USA Inc. (http://www.zend.com)
 * @license   http://framework.zend.com/license/new-bsd New BSD License
 * @package   Zend_Form
 */

namespace Zend\Form;

/**
 * @category   Zend
 * @package    Zend_Form
 */
interface FormFactoryAwareInterface
{
    /**
     * Compose a form factory into the object
     *
     * @param Factory $factory
     */
    public function setFormFactory(Factory $factory);
}
