<?php
// $Id: util.inc,v 1.1 2010/12/08 14:31:47 redijedi Exp $

/**
 * @file
 * Capsule API utility functions.
 */


/**
 * List of country names that can be supplied in the <country /> element of addresses. Alternatively ISO 3166-1 
 * alpha-2 or alpha-3 codes can be used.
 */
function capsule_list_countries() {
  return _capsule_request("/resource/country");
}

/**
 * List of ISO currencies currently supported by Capsule. These are the options available when creating new 
 * opportunities.
 */
function capsule_list_currencies() {
  return _capsule_request("/resource/currency");
}
