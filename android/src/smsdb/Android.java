package smsdb;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;
import org.qtproject.qt5.android.bindings.*;

public class Android {
    public Android()
    {
    }

    public boolean CopyToClipboard(String text)
    {
        try {
            int sdk = android.os.Build.VERSION.SDK_INT;
            if (sdk < android.os.Build.VERSION_CODES.HONEYCOMB) {
                android.text.ClipboardManager clipboard = (android.text.ClipboardManager)QtActivity.MainActivityRef().getSystemService(Context.CLIPBOARD_SERVICE);
                clipboard.setText(text);
            } else {
                android.content.ClipboardManager clipboard = (android.content.ClipboardManager)QtActivity.MainActivityRef().getSystemService(Context.CLIPBOARD_SERVICE);
                android.content.ClipData clip = android.content.ClipData.newPlainText("SMS Booklet", text);
                clipboard.setPrimaryClip(clip);
            }
        }

        catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public boolean SendText(String text)
    {
        try {
            Intent sendIntent = new Intent();
            sendIntent.setAction(Intent.ACTION_SEND);
            sendIntent.putExtra(Intent.EXTRA_TEXT, text);
            sendIntent.setType("text/plain");
            sendIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            QtActivity.MainActivityRef().startActivity(sendIntent);
        }
        
        catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        
        return true;
    }

    public boolean Notify(final String text, final int duration)
    {
        try {
            QtActivity.MainActivityRef().runOnUiThread(new Runnable() {
                public void run() {
                    Toast.makeText(QtActivity.MainActivityRef(), text, duration);
                }
            });
        }

        catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public boolean Release()
    {
        try {
        }
        
        catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        
        return true;
    }
}

