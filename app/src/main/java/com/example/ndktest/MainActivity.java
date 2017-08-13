package com.example.ndktest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.lang.reflect.Method;

public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";
    EditText count;
    private Class<NativeMemory> memoryClass;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        count = findViewById(R.id.count);
        memoryClass = NativeMemory.class;
    }

    public void native_heep(View view) {
        Button btn = (Button) view;
        try {
            Method method = memoryClass.getMethod(btn.getText().toString());
            int count = Integer.parseInt(this.count.getText().toString());
            for (int i = 0; i < count; i++) {
                method.invoke(null);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public void native_locref(View view) {
        Button btn = (Button) view;
        try {
            Method method = memoryClass.getMethod(btn.getText().toString(), int.class);
            int count = Integer.parseInt(this.count.getText().toString());
            method.invoke(null, count);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void log(int i){
        Log.e(TAG, "log: "+i );
    }

}
