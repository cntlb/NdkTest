package com.example.ndktest;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.text.Html;
import android.text.method.LinkMovementMethod;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.reflect.Method;

public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";
    private EditText count;
    private TextView logText;
    private Class<NativeMemory> memoryClass;
    private Class<NativeReference> referenceClass;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        count = (EditText) findViewById(R.id.count);
        logText = (TextView) findViewById(R.id.log_text);
        logText.setMovementMethod(new ScrollingMovementMethod());
        TextView link = (TextView) findViewById(R.id.source_link);
        link.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent  = new Intent(Intent.ACTION_VIEW);
                intent.setData(Uri.parse("https://github.com/cntlb/NdkTest/tree/master/app/src/main"));
                startActivity(intent);
            }
        });
        memoryClass = NativeMemory.class;
        referenceClass = NativeReference.class;
    }

    public void native_heep(View view) {
        Button btn = (Button) view;
        try {
            Method method = memoryClass.getMethod(btn.getText().toString());
            logText.setText(method.toString()+"\n");
            int count = Integer.parseInt(this.count.getText().toString());
            for (int i = 0; i < count; i++) {
                method.invoke(null);
            }
        } catch (Exception e) {
            e.printStackTrace();
            logout(e);
        }
    }


    public void native_locref(View view) {
        Button btn = (Button) view;
        try {
            String text = btn.getText().toString();
            text = text.replaceAll("^(.*)\\(.*\\)$", "$1");
            Method method = memoryClass.getMethod(text, int.class);
            logText.setText(method.toString()+"\n");
            int count = Integer.parseInt(this.count.getText().toString());
            method.invoke(null, count);
        } catch (Exception e) {
            e.printStackTrace();
            logout(e);
        }
    }

    private static void log(int i){
        Log.e(TAG, "log: "+i );
    }

    private void logout(Exception e){
        StringWriter writer = new StringWriter();
        e.printStackTrace(new PrintWriter(writer));
        logText.append(writer.getBuffer().toString());
    }

    public void native_more(View view) {
        Button btn = (Button) view;
        try {
            String text = btn.getText().toString();
            text = text.replaceAll("^(.*)\\(.*\\)$", "$1");
            Method method = referenceClass.getMethod(text, int.class);
            logText.setText(method.toString()+"\n");
            int count = Integer.parseInt(this.count.getText().toString());
            method.invoke(null, count);
        } catch (Exception e) {
            e.printStackTrace();
            logout(e);
        }
    }

    public void native_string(View view) {
        Button btn = (Button) view;
        try {
            String text = btn.getText().toString();
            text = text.replaceAll("^(.*)\\(.*\\)$", "$1");
            Method method = referenceClass.getMethod(text, String.class, int.class);
            logText.setText(method.toString()+"\n");
            int count = Integer.parseInt(this.count.getText().toString());
            method.invoke(null, "hello world from java", count);
        } catch (Exception e) {
            e.printStackTrace();
            logout(e);
        }
    }
}
