package com.mapbox.mapboxsdk.constants;


import org.junit.Test;

import static junit.framework.Assert.assertEquals;

public class StyleVersionTest {

  private static final double DELTA = 1e-15;

  @Test
  public void testSanity() {
    assertEquals("Style version should match, when upgrading, verify that integers.xml is updated",
      AppConstant.STYLE_VERSION,
      9,
      DELTA);
  }
}
