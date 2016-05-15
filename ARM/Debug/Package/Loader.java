package com.android.matrix;

public class Loader extends android.app.NativeActivity {
  static
  {
    System.loadLibrary("SDL2_arm");
    System.loadLibrary("SDL2_image_arm");
  }
}
