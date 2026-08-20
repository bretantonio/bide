package com.binclab.ide;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

import com.binclab.ide.databinding.ActivityMainBinding;

public class MainActivity extends Activity {

    // Used to load the 'ide' library on application startup.
    static {
        System.loadLibrary("ide");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.sample_text);
    }
}