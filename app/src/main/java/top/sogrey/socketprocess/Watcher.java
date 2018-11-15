package top.sogrey.socketprocess;

/**
 * 描述：
 * Created by Sogrey on 2018/11/15.
 */

public class Watcher {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    public native void createWatcher(String userId);
    public native void connectWatcher();

}
