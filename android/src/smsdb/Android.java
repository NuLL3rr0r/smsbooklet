package smsdb;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import org.qtproject.qt5.android.bindings.*;

public class Android {
    public Android()
    {
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

