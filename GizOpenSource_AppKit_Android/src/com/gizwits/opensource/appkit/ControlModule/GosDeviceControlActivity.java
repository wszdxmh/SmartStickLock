package com.gizwits.opensource.appkit.ControlModule;

import java.util.concurrent.ConcurrentHashMap;

import com.gizwits.gizwifisdk.api.GizWifiDevice;
import com.gizwits.gizwifisdk.enumration.GizWifiErrorCode;
import com.gizwits.gizwifisdk.listener.GizWifiDeviceListener;
import com.gizwits.opensource.appkit.R;
import com.gizwits.opensource.appkit.CommonModule.GosBaseActivity;

import android.R.integer;
import android.app.ActionBar;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;

public class GosDeviceControlActivity extends GosBaseActivity {

	/** The GizWifiDevice device */
	private GizWifiDevice device;

	/** The device status. */
	private ConcurrentHashMap<String, Object> deviceStatu;

	/** The Constant UPDATE_UI. */
	protected static final int UPDATE_UI = 2;

	/** The ActionBar actionBar */
	ActionBar actionBar;

	/** 开关门状态 **/
	private TextView tv_flag_switch;

	/** 开关门 **/
	private ImageView iv_opendoor;
	private ImageView iv_closedoor;
	private TextView tv_contrldoor;

	/** 反锁 **/
	private ImageView iv_pluslock;
	private ImageView iv_sublock;
	private TextView tv_latchdoor;

	/** 访客密码 **/
	private ImageView iv_userpasswd;

	/** 室外温度 **/
	private ImageView iv_temp;

	/** 儿童模式 **/
	private ImageView iv_children;
	private ImageView iv_man;
	private TextView tv_chirldern;

	/** 设置 **/
	private ImageView iv_setup;

	/** 电量提醒 **/
	private static int dump_energy = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_gos_device_control);
		initDevice();
		setActionBar(true, true, device.getProductName());
		initView();
		initEvents();
	}

	private void initView() {
		tv_flag_switch = (TextView) findViewById(R.id.tv_flag_switch);

		/** 开关门控件初始化 **/
		iv_opendoor = (ImageView) findViewById(R.id.iv_opendoor);
		iv_closedoor = (ImageView) findViewById(R.id.iv_closedoor);
		tv_contrldoor = (TextView) findViewById(R.id.tv_contrldoor);

		/** 反锁控件初始化 **/
		iv_pluslock = (ImageView) findViewById(R.id.iv_pluslock);
		iv_sublock = (ImageView) findViewById(R.id.iv_sublock);
		tv_latchdoor = (TextView) findViewById(R.id.tv_latchdoor);

		/** 访客密码控件 **/
		iv_userpasswd = (ImageView) findViewById(R.id.iv_userpasswd);

		/** 温度控件初始化 **/
		iv_temp = (ImageView) findViewById(R.id.iv_temp);

		/** 儿童模式 **/
		iv_children = (ImageView) findViewById(R.id.iv_children);
		iv_man = (ImageView) findViewById(R.id.iv_man);
		tv_chirldern = (TextView) findViewById(R.id.tv_chirldern);

		/** 设置 **/
		iv_setup = (ImageView) findViewById(R.id.iv_setup);

	}

	private void initDevice() {
		Intent intent = getIntent();
		device = (GizWifiDevice) intent.getParcelableExtra("GizWifiDevice");
		device.setListener(deviceListener);
		deviceStatu = new ConcurrentHashMap<String, Object>();
		Log.i("Apptest", device.getDid());
	}

	private void initEvents() {
		/** 开关门操作 **/
		iv_opendoor.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				iv_closedoor.setVisibility(View.VISIBLE);
				iv_opendoor.setVisibility(View.GONE);
				tv_contrldoor.setText("关门");
				sendCommand("switch_door", true);
			}
		});
		iv_closedoor.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				iv_opendoor.setVisibility(View.VISIBLE);
				iv_closedoor.setVisibility(View.GONE);
				tv_contrldoor.setText("开门");
				sendCommand("switch_door", false);
			}
		});

		/** 反锁操作 **/
		iv_pluslock.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				iv_pluslock.setVisibility(View.GONE);
				iv_sublock.setVisibility(View.VISIBLE);
				sendCommand("latch_lock", false);
				tv_latchdoor.setText("未反锁");
			}
		});
		iv_sublock.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				iv_sublock.setVisibility(View.GONE);
				iv_pluslock.setVisibility(View.VISIBLE);
				sendCommand("latch_lock", true);
				tv_latchdoor.setText("已反锁");
			}
		});

		/** 访客密码 **/
		iv_userpasswd.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				new AlertDialog.Builder(GosDeviceControlActivity.this)
						.setTitle("访客密码")
						.setMessage(
								deviceStatu.get("visitor_passwd").toString())
						.setPositiveButton("确定",
								new DialogInterface.OnClickListener() {

									@Override
									public void onClick(DialogInterface dialog,
											int which) {
										// TODO Auto-generated method stub

									}
								}).show();
			}
		});
		/** 室外温度 **/
		iv_temp.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				new AlertDialog.Builder(GosDeviceControlActivity.this)
						.setTitle("提示")
						.setMessage(
								"室外温度为："
										+ Float.parseFloat((deviceStatu
												.get("switch_manID").toString()))
										/ 10)
						.setPositiveButton("确定",
								new DialogInterface.OnClickListener() {

									@Override
									public void onClick(DialogInterface dialog,
											int which) {
										// TODO Auto-generated method stub
										sendCommand("random_passwd", true);
									}
								}).show();
			}
		});
		/** 儿童模式 **/
		iv_children.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				iv_children.setVisibility(View.GONE);
				iv_man.setVisibility(View.VISIBLE);
				tv_chirldern.setText("正常模式");
				sendCommand("children_mode", false);
			}
		});
		iv_man.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				iv_man.setVisibility(View.GONE);
				iv_children.setVisibility(View.VISIBLE);
				tv_chirldern.setText("儿童模式");
				sendCommand("children_mode", true);
			}
		});
		iv_setup.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				new AlertDialog.Builder(GosDeviceControlActivity.this)
						.setTitle("提示")
						.setMessage("设置功能尚未开放，敬请期待^-^")
						.setPositiveButton("确定",
								new DialogInterface.OnClickListener() {

									@Override
									public void onClick(DialogInterface dialog,
											int which) {
										// TODO Auto-generated method stub

									}
								}).show();
			}
		});

	}

	/**
	 * 发送指令
	 * 
	 * @param key
	 *            数据点对应的标识名
	 * @param value
	 *            需要改变的值
	 */
	public void sendCommand(String key, Object value) {
		int sn = 5;
		ConcurrentHashMap<String, Object> hashMap = new ConcurrentHashMap<String, Object>();
		hashMap.put(key, value);
		device.write(hashMap, sn);
		Log.i("Apptest", hashMap.toString());
	}

	/**
	 * 更新UI界面线程
	 */
	private Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
			case UPDATE_UI:
				updataUI();
				break;
			}
		};
	};

	@Override
	protected void onDestroy() {
		super.onDestroy();
		// 退出控制页面，取消设备订阅
		device.setSubscribe("4a7189d737f64b469863aa790f746f44", false);
	}

	/**
	 * 更新UI界面函数
	 */
	private void updataUI() {
		if ((Boolean) deviceStatu.get("Flag_switch") != null) {
			if ((Boolean) deviceStatu.get("Flag_switch") == true) {
				tv_flag_switch.setText("开");
			} else if ((Boolean) deviceStatu.get("Flag_switch") == false) {
				tv_flag_switch.setText("关");
			}
			if (deviceStatu.get("switch_door") != null) {
				if ((Boolean) deviceStatu.get("switch_door") == true) {
					iv_opendoor.setVisibility(View.GONE);
					iv_closedoor.setVisibility(View.VISIBLE);
					tv_contrldoor.setText("关门");
				} else if ((Boolean) deviceStatu.get("switch_door") == false) {
					iv_opendoor.setVisibility(View.VISIBLE);
					iv_closedoor.setVisibility(View.GONE);
					tv_contrldoor.setText("开门");
				}
			}
			if (deviceStatu.get("latch_lock") != null) {
				if ((Boolean) deviceStatu.get("latch_lock") == false) {
					iv_pluslock.setVisibility(View.GONE);
					iv_sublock.setVisibility(View.VISIBLE);
					tv_latchdoor.setText("未反锁");
				} else if ((Boolean) deviceStatu.get("latch_lock") == true) {
					iv_pluslock.setVisibility(View.VISIBLE);
					iv_sublock.setVisibility(View.GONE);
					tv_latchdoor.setText("已反锁");
				}
			}
			if (deviceStatu.get("dump_energy") != null) {
				if (dump_energy == 0
						&& (Integer.parseInt(deviceStatu.get("dump_energy")
								.toString()) < 10)) {
					dump_energy = 1;
					new AlertDialog.Builder(GosDeviceControlActivity.this)
							.setTitle("提示")
							.setMessage("剩余电量不足10%,请及时充电！^-^")
							.setPositiveButton("确定",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											// TODO Auto-generated method stub

										}
									}).show();
				}
			}
		}
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			this.finish();
			break;
		}
		return super.onOptionsItemSelected(item);
	}

	/** 设备** */
	private GizWifiDeviceListener deviceListener = new GizWifiDeviceListener() {
		// 接收数据回调
		public void didReceiveData(GizWifiErrorCode result,
				GizWifiDevice device,
				ConcurrentHashMap<String, Object> dataMap, int sn) {
			Message msg = new Message();
			if (GizWifiErrorCode.GIZ_SDK_SUCCESS != result) {
				// 查询成功
			} else {
				// 查询失败
			}
			// 已定义的设备数据点，有布尔、数值和枚举型数据
			if (dataMap.get("data") != null) {
				deviceStatu = (ConcurrentHashMap<String, Object>) dataMap
						.get("data");
				// 根据标识名，在回调的map中找到设备上报的值
				if (deviceStatu.get("switch_door") != null) {
					// 根据设备上报的值更改按钮的图标
					msg.what = UPDATE_UI;
					handler.sendMessage(msg);
				}
			}
		}
	};
}
