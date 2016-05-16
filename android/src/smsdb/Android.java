package smsdb;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;
import java.lang.*;
import java.lang.reflect.*;
import org.qtproject.qt5.android.bindings.*;

public class Android {
    public Android()
    {
    }

    public boolean CopyToClipboard(String text)
    {
        try {
            int sdk = android.os.Build.VERSION.SDK_INT;
            if (sdk < 11) {     // android.os.Build.VERSION_CODES.HONEYCOMB
                Object clipboard = QtActivity.MainActivityRef().getSystemService(Context.CLIPBOARD_SERVICE);
                Method setText = clipboard.getClass().getMethod("setText", CharSequence.class);
                setText.invoke(clipboard, text);
            } else {
                Object clipboard = QtActivity.MainActivityRef().getSystemService(Context.CLIPBOARD_SERVICE);
                Class clipClass = Class.forName("android.content.ClipData");
                Method newPlainText = clipClass.getMethod("newPlainText", CharSequence.class, CharSequence.class);
                Object clip = newPlainText.invoke((Object)clipClass, new Object[] {"SMS Booklet", text});
                Method setPrimaryClip = clipboard.getClass().getMethod("setPrimaryClip", clip.getClass());
                setPrimaryClip.invoke(clipboard, clip);
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

