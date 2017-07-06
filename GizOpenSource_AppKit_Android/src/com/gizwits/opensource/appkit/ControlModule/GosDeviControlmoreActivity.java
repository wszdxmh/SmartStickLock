package com.gizwits.opensource.appkit.ControlModule;

import java.util.concurrent.ConcurrentHashMap;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageView;

import com.gizwits.gizwifisdk.api.GizWifiDevice;
import com.gizwits.gizwifisdk.enumration.GizWifiErrorCode;
import com.gizwits.gizwifisdk.listener.GizWifiDeviceListener;
import com.gizwits.opensource.appkit.R;
import com.gizwits.opensource.appkit.CommonModule.GosBaseActivity;

public class GosDeviControlmoreActivity extends GosDeviceControlActivity{
	
//	/** The GizWifiDevice device */
//	public GizWifiDevice device;
//	
//	/** The device status. */
//	public ConcurrentHashMap<String, Object> deviceStatu;
//	
//	/****/
//	private ImageView iv_lock;
//	private ImageView iv_unlock;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_gos_device_control_more);
//		initDevice();
//		initView();
//		initEvents();
	}

//	
//	private void initView() {
//		iv_lock = (ImageView) findViewById(R.id.iv_lock);
//		iv_unlock = (ImageView) findViewById(R.id.iv_unlock);
//	}
//	
//	private void initDevice() {
//		Intent intent = getIntent();
//		device = (GizWifiDevice) intent.getParcelableExtra("GizWifiDevice");
//		device.setListener(deviceListener);
//		deviceStatu = new ConcurrentHashMap<String, Object>();
//		Log.i("Apptest", device.getDid());
//	}
//	
//	private void initEvents() {
//		iv_lock.setOnClickListener(new OnClickListener() {
//			@Override
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				sendCommand("switch_door", true);
//				iv_lock.setVisibility(View.GONE);
//				iv_unlock.setVisibility(View.VISIBLE);
//			}
//		});
//		
//		iv_unlock.setOnClickListener(new OnClickListener() {
//			@Override
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				sendCommand("switch_door", false);
//				iv_unlock.setVisibility(View.GONE);
//				iv_lock.setVisibility(View.VISIBLE);
//			}
//		});
//	}
//	
//	/**
//	 * 发送指令
//	 * 
//	 * @param key
//	 *            数据点对应的标识名
//	 * @param value
//	 *            需要改变的值
//	 */
//	public void sendCommand(String key, Object value) {
//		int sn = 5;
//		ConcurrentHashMap<String, Object> hashMap = new ConcurrentHashMap<String, Object>();
//		hashMap.put(key, value);
//		device.write(hashMap, sn);
//		Log.i("Apptest", hashMap.toString());
//	}
//
//	/** 设备** */
//	public GizWifiDeviceListener deviceListener = new GizWifiDeviceListener() {
//		// 接收数据回调
//		public void didReceiveData(GizWifiErrorCode result,
//				GizWifiDevice device,
//				ConcurrentHashMap<String, Object> dataMap, int sn) {
//			Message msg = new Message();
//			if (GizWifiErrorCode.GIZ_SDK_SUCCESS != result) {
//				// 查询成功
//			} else {
//				// 查询失败
//			}
//			// 已定义的设备数据点，有布尔、数值和枚举型数据
//			if (dataMap.get("data") != null) {
//				deviceStatu = (ConcurrentHashMap<String, Object>) dataMap
//						.get("data");
//				// 根据标识名，在回调的map中找到设备上报的值
//				if (deviceStatu.get("switch_door") != null) {
//					// 根据设备上报的值更改按钮的图标
//					msg.what = UPDATE_UI;
//					
//				}
//			}
//		}
//	};
	
}
