package com.gizwits.opensource.appkit.ControlModule;

import java.util.concurrent.ConcurrentHashMap;

import android.app.ActionBar;
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


public class GosDeviceConrtolplayActivity extends GosBaseActivity{
	
	
	/** The GizWifiDevice device */
	private GizWifiDevice device;
	
	/** The ActionBar actionBar */
	ActionBar actionBar;
	
	/** The device status. */
	private ConcurrentHashMap<String, Object> deviceStatu;
	
	/** The Constant UPDATE_UI. */
	protected static final int UPDATE_UI = 2;
	
	/****/
	private ImageView iv_lock;
	private ImageView iv_unlock;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_gos_device_control_paly);
//		setActionBar(true, true, device.getProductName());
//		initDevice();
//		initView();
//		initEvents();
	}
	
}
