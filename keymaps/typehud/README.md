# Typehud Keymap

VIA compatible keymap that displays a live wpm HUD on your OLED.

<https://user-images.githubusercontent.com/2514771/166553352-7ce23eca-57a5-40fc-af41-73363eec2f40.mp4>

## Configuration

Configuration options (other than the keymap itself) can be found in `typehud/config.h`.

### OLED Orientation

To change the OLED orientation update the value of `OLED_ROTATE` to one of the following:

-   `OLED_ROTATION_0`
-   `OLED_ROTATION_90`
-   `OLED_ROTATION_180`
-   `OLED_ROTATION_270`

### Graph Type

By default the graph is filled. For a non-filled graph remove or comment out the following line:

```c
#define TYPEHUD_FILLGRAPH
```

### Keyboard Matrix Orientation

To change the keyboard matrix orientation add one of the following:

-   `TYPEHUD_MATRIX_ROTATE_90`
-   `TYPEHUD_MATRIX_ROTATE_180`
-   `TYPEHUD_MATRIX_ROTATE_270`
