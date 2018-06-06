/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.gl2v8;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;

import java.io.InputStream;
import java.nio.ByteBuffer;


public class GL2JNIActivity extends Activity {
    private static final String TAG = "GL2JNIActivity";
    GL2JNIView mView;

    @Override
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        com.android.gl2v8.GL2JNILib.loadShaderSource(getAssets());
        //传递image到C++
        Bitmap bitmap = getBitmap("image/1.png");
        int sizeBitmap = bitmap.getByteCount();
        byte[] bytesArray = new byte[sizeBitmap];
        ByteBuffer byteBuffer = ByteBuffer.wrap(bytesArray);
        bitmap.copyPixelsToBuffer(byteBuffer);
        GL2JNILib.initImage(bytesArray,bitmap.getWidth(),bitmap.getHeight());

        mView = new GL2JNIView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        GL2JNILib.onDestory();
    }

    public Bitmap getBitmap(String name) {
        Bitmap bitmap;
        try {
            //从assets目录下读文件
            String fileName = name;// 文件名字
            InputStream in = getResources().getAssets().open(fileName);
            int length = in.available();
            byte[] buffer = new byte[length];
            in.read(buffer);
            //转换为Bitmap
            BitmapFactory.Options opts = new BitmapFactory.Options();
            bitmap = BitmapFactory.decodeByteArray(buffer, 0, buffer.length, opts);
            return  bitmap;

        }catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
