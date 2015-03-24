package org.rfcx.sentinel;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.util.SerialInputOutputManager;

import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class SentinelActivity extends Activity {
    private final String TAG = SentinelActivity.class.getSimpleName();
    //private final long BEGIN_FLAG = 0xBB;
    //private final long END_FLAG = 0xEE;

    private static UsbSerialPort sPort = null;

    private TextView mStatus;
    private TextView mInputCurrentValue;
    private TextView mInputVoltageValue;
    private TextView mOutputCurrentValue;
    private TextView mOutputVoltageValue;
    private TextView mTemperatureValue;
    private TextView mHumidityValue;

    private final ExecutorService mExecutor = Executors.newSingleThreadExecutor();

    private SerialInputOutputManager mSerialIoManager;

    private final SerialInputOutputManager.Listener mListener = new SerialInputOutputManager.Listener() {

        @Override
        public void onRunError(Exception e) {
            Log.d(TAG, "Runner stopped.");
        }

        @Override
        public void onNewData(final byte[] data) {
            SentinelActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    SentinelActivity.this.updateReceivedData(data);
                }
            });
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.sentinel);
        mStatus = (TextView) findViewById(R.id.status);
        mInputCurrentValue = (TextView)findViewById(R.id.inputCurrentValue);
        mInputVoltageValue = (TextView)findViewById(R.id.inputVoltageValue);
        mOutputCurrentValue = (TextView)findViewById(R.id.outputCurrentValue);
        mOutputVoltageValue = (TextView)findViewById(R.id.outputVoltageValue);
        mTemperatureValue = (TextView)findViewById(R.id.temperatureValue);
        mHumidityValue = (TextView)findViewById(R.id.humidityValue);
    }

    @Override
    protected void onPause() {
        super.onPause();
        stopIoManager();
        if (sPort != null) {
            try {
                sPort.close();
            } catch (IOException e) {
                // Ignore.
            }
            sPort = null;
        }
        finish();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "Resumed, port=" + sPort);
        if (sPort == null) {
            mStatus.setText("No serial device.");
            mStatus.setTextColor(0xFFFF5F69);
        } else {
            final UsbManager usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);

            UsbDeviceConnection connection = usbManager.openDevice(sPort.getDriver().getDevice());
            if (connection == null) {
                mStatus.setText("Opening device failed");
                mStatus.setTextColor(0xFFFF5F69);
                return;
            }

            try {
                sPort.open(connection);

                //115200 Baud rate
                //8 data bits
                //1 stop bit
                //No parity bit
                sPort.setParameters(115200, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);
            } catch (IOException e) {
                Log.e(TAG, "Error setting up device: " + e.getMessage(), e);
                mStatus.setText("Error opening device: " + e.getMessage());
                mStatus.setTextColor(0xFFFF5F69);
                try {
                    sPort.close();
                } catch (IOException e2) {
                    // Ignore.
                }
                sPort = null;
                return;
            }
            mStatus.setText("Device: " + sPort.getClass().getSimpleName());
            mStatus.setTextColor(0xFFFFFFFF);
        }
        onDeviceStateChange();
    }

    private void stopIoManager() {
        if (mSerialIoManager != null) {
            Log.i(TAG, "Stopping io manager ..");
            mSerialIoManager.stop();
            mSerialIoManager = null;
        }
    }

    private void startIoManager() {
        if (sPort != null) {
            Log.i(TAG, "Starting io manager ..");
            mSerialIoManager = new SerialInputOutputManager(sPort, mListener);
            mExecutor.submit(mSerialIoManager);
        }
    }

    private void onDeviceStateChange() {
        stopIoManager();
        startIoManager();
    }

    private void updateReceivedData(byte[] data) {
        final String message = new String(data);

        /*
            values[0] = Input Current (mA)
            values[1] = Input Voltage (V)
            values[2] = Output Current (mA)
            values[3] = Output Voltage (V)
            values[4] = Temperature (C)
            values[5] = Humidity (RH)
         */
        String[] values = message.split(",");

        //Ensure the length is correct (8), and use framing
        //if((values.length >= 8) && (Long.parseLong(values[0]) == BEGIN_FLAG) && (Long.parseLong(values[7]) == END_FLAG)) {
        if(values.length >= 8) {
                mStatus.setTextColor(Color.GREEN);
                mInputCurrentValue.setText(values[1] + " mA");
                mInputVoltageValue.setText(values[2] + " V");
                mOutputCurrentValue.setText(values[3] + " mA");
                mOutputVoltageValue.setText(values[4] + "V");
                mTemperatureValue.setText(values[5] + " C");
                mHumidityValue.setText(values[6] + " % RH");
        } else {
            mStatus.setTextColor(Color.RED);
            //mStatus.setText(message);
            //mInputCurrentValue.setText("Length: " + values.length);
        }
    }

    /**
     * Starts the activity, using the supplied driver instance.
     *
     * @param context the Context
     * @param port the USB Serial port
     */
    static void show(Context context, UsbSerialPort port) {
        sPort = port;
        final Intent intent = new Intent(context, SentinelActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP | Intent.FLAG_ACTIVITY_NO_HISTORY);
        context.startActivity(intent);
    }
}
