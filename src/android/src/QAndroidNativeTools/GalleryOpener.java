package QAndroidNativeTools;

import android.content.Context;
import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;

import android.app.Activity;


import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class GalleryOpener
{
    /*public static void testDEBUGGING_LOG() //how to print debugging message
    {
        System.out.println("PRINTED FROM JAVA");
    }*/

    private static String[] addElement(String myarray[],String ele)
    {
        int size = myarray.length;
        int i;
        String newArray[] = new String[size+1];
        for(i=0; i < size;i++)
        {
            newArray[i] = myarray[i];
        }
        newArray[size] = ele;

        return newArray;
    }

    public static String getRealPathFromURI(Context context, Uri contentUri)
    {
        Cursor cursor = null;
        try
        {
            String[] proj = { MediaStore.Images.Media.DATA };
            cursor = context.getContentResolver().query(contentUri,  proj, null, null, null);
            int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
            cursor.moveToFirst();
            return cursor.getString(column_index);
        }
        finally
        {
            if (cursor != null)
            {
                cursor.close();
            }
        }
    }

    public static String[] onActivityResult(int requestCode, int resultCode, Intent data,Context context)
    {
        String[] output = new String[0];

        //if(requestCode == Activity.SELECT_PICTURES)
        //{
            if(resultCode == Activity.RESULT_OK)
            {
                if(data.getClipData() != null)
                {
                    int count = data.getClipData().getItemCount(); //evaluate the count before the for loop --- otherwise, the count is evaluated every loop.
                    for(int i = 0; i < count; i++)
                    {
                        Uri selectedImage = data.getClipData().getItemAt(i).getUri();
                        String picturePath = getRealPathFromURI(context,selectedImage);

                        System.out.println("picturePath +"+ picturePath );  //path of sdcard

                        output = addElement(output,picturePath);
                        //do something with the image (save it to some directory or whatever you need to do with it here)
                    }
                }

                else if(data.getData() != null)
                {
                    /*Uri imageUri = Uri.parse(data.getData().getPath());
                    String imagePath = PathUtil.getPath(getApplicationContext(),data.getData());
                    output = addElement(output,imagePath);*/
                    //do something with the image (save it to some directory or whatever you need to do with it here)
                    Uri selectedImage = data.getData();

                    String picturePath = getRealPathFromURI(context,selectedImage);

                    System.out.println("picturePath +"+ picturePath );  //path of sdcard
                    output = addElement(output,picturePath);
                }
            }
        //}
        System.out.println(".");
        System.out.println(".");
        System.out.println("Output size : " + output.length+ "  --------------------------------------------------------------------------------");
        int i;
        for(i = 0; i < output.length;i++)
        {
            System.out.println("Element : " + output[i]);
        }
        System.out.println(".");
        System.out.println(".");

        return output;
    }
}//class Utils
