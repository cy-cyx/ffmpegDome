package com.example.ffmpegdome.utils

import android.annotation.SuppressLint
import android.app.Activity
import android.content.Context
import android.content.pm.ActivityInfo
import android.graphics.Bitmap
import android.graphics.Color
import android.graphics.Point
import android.graphics.Rect
import android.os.Build
import android.util.DisplayMetrics
import android.view.KeyCharacterMap
import android.view.KeyEvent
import android.view.Surface
import android.view.View
import android.view.ViewConfiguration
import android.view.WindowManager

object ScreenUtils {
    private var mDM: DisplayMetrics? = null
    private var mDensity = 0f
    private var mWidthPixels = 0
    private var mHeightPixels = 0
    private var mWindowWidthPixels = 0
    private var mWindowHeightPixels = 0
    private var mStatusBarHeight: Int? = null
    private var mNavigationBarHeight: Int? = null
    private var mRealWidthPixels = 0
    private var mRealHeightPixels = 0
    private var mInit = false

    fun init(activity: Activity?) {
        if (activity == null || mInit) {
            return
        }
        mDM = activity.resources.displayMetrics

        mDensity = mDM!!.density
        mWidthPixels = Math.min(mDM!!.widthPixels, mDM!!.heightPixels)
        mHeightPixels = Math.max(mDM!!.widthPixels, mDM!!.heightPixels)
        mWindowWidthPixels = getWindowWidth(activity)
        mWindowHeightPixels = getWindowHeight(activity)

        val windowManager = activity.windowManager
        val display = windowManager.defaultDisplay
        val p = Point()
        display.getRealSize(p)
        mRealWidthPixels = p.x
        mRealHeightPixels = p.y
        mInit = true
    }

    fun getDisplayMetrics(): DisplayMetrics? {
        return mDM
    }

    fun getDensity(): Float {
        return mDensity
    }

    fun getWidthPixels(): Int {
        return mWidthPixels
    }

    fun getHeightPixels(): Int {
        return mHeightPixels
    }

    fun getRealWidthPixels(): Int {
        return mRealWidthPixels
    }

    fun getRealHeightPixels(): Int {
        return mRealHeightPixels
    }

    fun getWindowWidthPixels(): Int {
        return mWindowWidthPixels
    }

    fun getWindowHeightPixels(): Int {
        return mWindowHeightPixels
    }

    fun px2dp(px: Int): Int {
        return (0.5f + px / mDensity).toInt()
    }

    fun dp2px(dip: Float): Int {
        return (0.5f + mDensity * dip).toInt()
    }

    fun percentHeight(percent: Float): Int {
        return (percent * getHeightPixels()).toInt()
    }

    fun percentWidth(percent: Float): Int {
        return (percent * getRealWidthPixels()).toInt()
    }

    /**
     * 获取状态栏高度
     *
     * @param activity
     * @return
     */
    private fun getStatusBarHeightInner(activity: Activity?): Int {
        if (activity == null) {
            return 0
        }
        if (mStatusBarHeight != null) {
            return mStatusBarHeight!!
        }
        var statusBarHeight = 0
        try {
            val c = Class.forName("com.android.internal.R\$dimen")
            val obj = c.newInstance()
            val field = c.getField("status_bar_height")
            val x = Integer.parseInt(field.get(obj).toString())
            statusBarHeight = activity.resources.getDimensionPixelSize(x)
        } catch (e: java.lang.Exception) {
            val rect = Rect()
            activity.window.decorView.getWindowVisibleDisplayFrame(rect)
            statusBarHeight = rect.top
        }
        mStatusBarHeight = statusBarHeight
        return statusBarHeight
    }

    private fun getWindowWidth(activity: Activity): Int {
        val decor = activity.window.decorView
        return decor.measuredWidth
    }

    /**
     * 获取窗口高度
     */
    private fun getWindowHeight(activity: Activity): Int {
        val decor = activity.window.decorView
        return decor.measuredHeight
    }

    /**
     * 设置屏幕为横屏
     *
     * 还有一种就是在Activity中加属性android:screenOrientation="landscape"
     *
     * 不设置Activity的android:configChanges时，切屏会重新调用各个生命周期，切横屏时会执行一次，切竖屏时会执行两次
     *
     * 设置Activity的android:configChanges="orientation"时，切屏还是会重新调用各个生命周期，切横、竖屏时只会执行一次
     *
     * 设置Activity的android:configChanges="orientation|keyboardHidden|screenSize"（4.0以上必须带最后一个参数）时
     * 切屏不会重新调用各个生命周期，只会执行onConfigurationChanged方法
     *
     * @param activity activity
     */
    fun setLandscape(activity: Activity) {
        activity.requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE
    }

    /**
     * 设置屏幕为竖屏
     *
     * @param activity activity
     */
    fun setPortrait(activity: Activity) {
        activity.requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT
    }

    /**
     * 获取屏幕旋转角度
     *
     * @param activity activity
     * @return 屏幕旋转角度
     */
    fun getScreenRotation(activity: Activity): Int {
        when (activity.windowManager.defaultDisplay.rotation) {
            Surface.ROTATION_0 -> return 0
            Surface.ROTATION_90 -> return 90
            Surface.ROTATION_180 -> return 180
            Surface.ROTATION_270 -> return 270
            else -> return 0
        }
    }

    /**
     * 获取当前屏幕截图，包含状态栏
     *
     * @param activity activity
     * @return Bitmap
     */
    fun captureWithStatusBar(activity: Activity): Bitmap {
        val view = activity.window.decorView
        view.isDrawingCacheEnabled = true
        view.buildDrawingCache()
        val bmp = view.drawingCache
        val dm = DisplayMetrics()
        activity.windowManager.defaultDisplay.getMetrics(dm)
        val ret = Bitmap.createBitmap(bmp, 0, 0, dm.widthPixels, dm.heightPixels)
        view.destroyDrawingCache()
        return ret
    }

    /**
     * 获取状态栏高度
     *
     * @param mActivity
     * @return
     */
    fun getStatusBarHeight(mActivity: Activity): Int {
        val resources = mActivity.resources
        val resourceId = resources.getIdentifier("status_bar_height", "dimen", "android")
        return resources.getDimensionPixelSize(resourceId)
    }

    /**
     * 获取状态栏高度
     *
     * @param mActivity
     * @return
     */
    fun getStatusBarHeight(context: Context): Int {
        val resources = context.resources
        val resourceId = resources.getIdentifier("status_bar_height", "dimen", "android")
        return resources.getDimensionPixelSize(resourceId)
    }

    /**
     * 获取底部导航栏高度
     *
     * @param mActivity
     * @return
     */
    fun getNavigationBarHeight(context: Context): Int {
        if (mNavigationBarHeight != null) {
            return mNavigationBarHeight!!
        }
        var height = 0
        if (hasNavigationBar(context)) {
            val resources = context.resources
            val resourceId = resources.getIdentifier("navigation_bar_height", "dimen", "android")
            height = resources.getDimensionPixelSize(resourceId)
        }
        mNavigationBarHeight = height
        return height
    }

    /**
     * 获取是否存在底部导航栏
     *
     * @param context
     * @return
     */
    fun hasNavigationBar(context: Context): Boolean {
        var hasNavigationBar = false
        val rs = context.resources
        val id = rs.getIdentifier("config_showNavigationBar", "bool", "android")
        if (id > 0) {
            hasNavigationBar = rs.getBoolean(id)
        }
        try {
            val systemPropertiesClass = Class.forName("android.os.SystemProperties")
            val m = systemPropertiesClass.getMethod("get", String::class.java)
            val navBarOverride = m.invoke(systemPropertiesClass, "qemu.hw.mainkeys") as String
            if ("1" == navBarOverride) {
                hasNavigationBar = false
            } else if ("0" == navBarOverride) {
                hasNavigationBar = true
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }

        return hasNavigationBar
    }

    fun setWindowStatusBarColor(activity: Activity, colorResId: Int) {
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                val window = activity.window
                window.statusBarColor = colorResId
                //底部导航栏
                //window.setNavigationBarColor(activity.getResources().getColor(colorResId));
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    @SuppressLint("NewApi")
    //隐藏虚拟键
    fun hideSystemUI(view: View) {
        view.systemUiVisibility = (View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
            or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar

            or View.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar

            or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY)
    }

    @SuppressLint("NewApi")
    //显示虚拟键
    fun showSystemUI(view: View) {
        view.systemUiVisibility = (View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            or View.SYSTEM_UI_FLAG_VISIBLE
            or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY)
    }

    fun setTransparentBar(activity: Activity) {
        if (Build.VERSION.SDK_INT > 11 && Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) { // lower api
            val v = activity.window.decorView
            v.systemUiVisibility = View.GONE
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            val window = activity.window
            window.decorView.systemUiVisibility =
                View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY or
                    View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                window.navigationBarColor = Color.TRANSPARENT
                window.statusBarColor = Color.TRANSPARENT
            }
        }
    }

    fun setStatusColor(activity: Activity, colorId: Int) {
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                val window = activity.window
                window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS)
                window.decorView.systemUiVisibility =
                    View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS)
                window.statusBarColor = colorId
            } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                val window = activity.window
                window.statusBarColor = colorId
            }
        } catch (e: Exception) {
        }
    }

    fun setTranslucentNavigation(activity: Activity) {
        try {
            if (Build.VERSION.SDK_INT > 11 && Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) { // lower api
                val v = activity.window.decorView
                v.systemUiVisibility = View.GONE
            } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                val window = activity.window
                window.decorView.systemUiVisibility =
                    View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY or
                        View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                window.addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION) //设置透明导航
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    window.navigationBarColor = Color.TRANSPARENT
                }
            }
        } catch (e: Exception) {
        }
    }

    fun isNavigationBarShow(activity: Activity): Boolean {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            val display = activity.windowManager.defaultDisplay
            val size = Point()
            val realSize = Point()
            display.getSize(size)
            display.getRealSize(realSize)
            return realSize.y != size.y
        } else {
            val menu = ViewConfiguration.get(activity).hasPermanentMenuKey()
            val back = KeyCharacterMap.deviceHasKey(KeyEvent.KEYCODE_BACK)
            return !(menu || back)
        }
    }

}