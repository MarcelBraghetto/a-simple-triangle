package io.github.marcelbraghetto.asimpletriangle;

import org.libsdl.app.SDLActivity;

public class MainActivity extends SDLActivity {
    @Override
    protected String[] getLibraries() {
        return new String[]{
                "hidapi",
                "SDL2",
                "a-simple-triangle"
        };
    }
}
