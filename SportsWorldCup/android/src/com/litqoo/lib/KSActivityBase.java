package com.litqoo.lib;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.StringTokenizer;
import java.util.UUID;
import java.util.concurrent.Semaphore;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.RemoteException;
import android.os.Vibrator;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;


import com.example.android.trivialdrivesample.util.IabHelper;
import com.example.android.trivialdrivesample.util.IabResult;
import com.google.ads.Ad;
import com.google.ads.AdListener;
import com.google.ads.AdRequest.ErrorCode;
import com.google.ads.AdView;
import com.litqoo.SportsWorldCupForKaKao.R;

abstract class LQRunnable implements Runnable {
	protected final String totalSource;
	protected final int delekey;

	LQRunnable(String str, int k) {
		totalSource = str;
		delekey = k;
	}
}


public class KSActivityBase extends KSInAppBase implements AdListener {
	protected Cocos2dxGLSurfaceView mGLView;
	private final Semaphore semaphore = new Semaphore(1);
	protected Handler handler = new Handler();
	protected String deviceId;

	private WebView browser = null;
	private WebView etcWebView = null;
	private static final int MARKET_GOOGLE = 1;
	private static final int MARKET_TSTORE = 2;
	private final int MARKET_TYPE = MARKET_GOOGLE; // /< default setting.

	Handler getHandler() {
		return handler;
	}

	class Size {
		public Size(int w, int h) {
			width = w;
			height = h;
		}

		public int width;
		public int height;
	}

	private int getLeftMargin() {
		Size b = getFitScreen();
		DisplayMetrics displayMetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
		int deviceWidth = displayMetrics.widthPixels;

		return (deviceWidth - b.width) / 2;
	}

	private int getBottomMargin() {
		Size b = getFitScreen();
		DisplayMetrics displayMetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
		int deviceHeight = displayMetrics.heightPixels;

		return (deviceHeight - b.height) / 2;
	}

	private Size getFitScreen() {
		DisplayMetrics displayMetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
		int deviceWidth = displayMetrics.widthPixels;
		int deviceHeight = displayMetrics.heightPixels;
		double C1 = deviceWidth / 480.;
		int screenFitWidth, screenFitHeight;
		if (320 * C1 > deviceHeight) {
			double C2 = deviceHeight / 320.;
			screenFitWidth = (int) (480 * C2);
			screenFitHeight = deviceHeight;
		} else if (320 * C1 == deviceHeight) {
			screenFitHeight = deviceHeight;
			screenFitWidth = deviceWidth;
		} else {
			screenFitWidth = deviceWidth;
			screenFitHeight = (int) (480 * C1);
		}
		return new Size(screenFitWidth, screenFitHeight);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		final TelephonyManager tm = (TelephonyManager) getBaseContext()
				.getSystemService(Context.TELEPHONY_SERVICE);
		final String tmDevice, tmSerial, androidId;
		tmDevice = "" + tm.getDeviceId();
		tmSerial = "" + tm.getSimSerialNumber();
		androidId = ""
				+ android.provider.Settings.Secure.getString(
						getContentResolver(),
						android.provider.Settings.Secure.ANDROID_ID);
		UUID deviceUuid = new UUID(androidId.hashCode(),
				((long) tmDevice.hashCode() << 32) | tmSerial.hashCode());
		deviceId = deviceUuid.toString();

	}

	protected void setAD() {
		activity = this;

		// AdView adView = (AdView)this.findViewById(R.id.adView);
		// adView.loadAd(new AdRequest());

	}

	private static Context activity;

	// //////////////////////////////////////////////////////
	protected static Object getActivity() {
		// Log.i("cppCall", "return activity~~~~!!!");
		return activity;
	}

	// //////////////////////////////////////////////////////////////////////////////////
	private class ProcessWebTask extends AsyncTask<String, Void, String> {
		private int paramCount = 0;
		private String recvedURL = null;
		private int _x, _y, _sx, _sy;
		private Context _this;

		public void setContext(Context ct) {
			_this = ct;
		}

		@Override
		protected void onCancelled() {
			super.onCancelled();
		}

		@Override
		protected void onPostExecute(String result) {
			try {
				semaphore.acquire();
				if (paramCount == 6) {
					int x, y, sx, sy;
					String url = recvedURL;
					x = _x;
					y = _y;
					sx = _sx;
					sy = _sy;
					// Context _this = this;
					String totalSource = result;
					if (totalSource.length() > 100) {
						Size mySize = getFitScreen();
						int left_margin = getLeftMargin();
						int bottom_margin = getBottomMargin();

						double xRatio = mySize.width / 480.;
						double yRatio = mySize.height / 320.;

						FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
								(int) (sx * xRatio), (int) (sy * yRatio),
								Gravity.TOP);
						params.setMargins((int) (x * xRatio) + left_margin,
								(int) (y * yRatio) + bottom_margin, 0, 0);

						browser = new WebView(_this);
						browser.setInitialScale((int) (xRatio * 100));
						browser.setWebViewClient(new WebViewClient() {
							@Override
							public boolean shouldOverrideUrlLoading(
									WebView view, String url) {

								if (url.matches("^market.*")) {
									Intent market = new Intent(
											Intent.ACTION_VIEW, Uri.parse(url));
									startActivity(market);
								} else
									view.loadUrl(url);
								return true;
							}
						});
						browser.getSettings().setLoadWithOverviewMode(true);
						browser.getSettings().setUseWideViewPort(false);
						browser.setBackgroundColor(0);
						browser.getSettings().setBuiltInZoomControls(false);
						browser.getSettings().setSupportZoom(false);
						browser.setLayoutParams(params);
						browser.setHorizontalScrollBarEnabled(false);
						browser.setVerticalScrollBarEnabled(false);
						// browser.loadUrl(url);
						// totalSource = "<font color=\"red\">test</font>";
						// totalSource =
						// "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> <meta name=\"format-detection\" content=\"telephone=no\"><script> document.ontouchmove = function(event){        event.preventDefault();    }    document.body.style.webkitTouchCallout='none';</script></head><body leftmargin=0 topmargin=0><table width=100% height=100% border=0 cellspacing=0 cellpadding=0 style='font-size:10px;'><tr><td colspan=4 bgcolor=black align=center><table width=100% height=100% border=0 cellspacing=0 cellpadding=0 style='font-size:10px;color:white'><tr><td align=left width=20%> </td><td align=center width=60%><b><font color=white> Top 10 Countries In RealTime</font></b></td><td align=right width=20%><a href=rankView.php?cmode=view&mode=now><font color=white>Rank▶</font></a></td></tr></table></td></tr><tr align=center bgcolor=yellow><td>1</td><td><img src=flag/cn.png></td><td>18 Shooters</td><td>77086 pt</td></tr><tr align=center bgcolor=eeeeee><td>2</td><td><img src=flag/us.png></td><td>23 Shooters</td><td>74198 pt</td></tr><tr align=center bgcolor=EEBB66><td>3</td><td><img src=flag/fr.png></td><td>4 Shooters</td><td>38802 pt</td></tr><tr align=center bgcolor=00ABF2><td>4</td><td><img src=flag/kr.png></td><td>11 Shooters</td><td>24784 pt</td></tr><tr align=center bgcolor=00B4FF><td>5</td><td><img src=flag/jp.png></td><td>6 Shooters</td><td>17623 pt</td></tr><tr align=center bgcolor=00ABF2><td>6</td><td><img src=flag/il.png></td><td>3 Shooters</td><td>11544 pt</td></tr><tr align=center bgcolor=00B4FF><td>7</td><td><img src=flag/hk.png></td><td>4 Shooters</td><td>9477 pt</td></tr><tr align=center bgcolor=00ABF2><td>8</td><td><img src=flag/it.png></td><td>3 Shooters</td><td>8582 pt</td></tr><tr align=center bgcolor=00B4FF><td>9</td><td><img src=flag/si.png></td><td>1 Shooters</td><td>6199 pt</td></tr><tr align=center bgcolor=00ABF2><td>10</td><td><img src=flag/tr.png></td><td>3 Shooters</td><td>5103 pt</td></tr><tr><td colspan=4 bgcolor=black align=center><table width=100% height=100% border=0 cellspacing=0 cellpadding=0 style='font-size:10px;color:white'><tr><td align=left width=30%> </td><td align=center width=40%><b><font color=white>ALL SHOOTERS:93</font></b></td><td align=right width=30%><a href=\"mailto:?subject=Let's Play BasketWorldCup togather&body=Download it in AppStore<br><a href=http://itunes.apple.com/app/id512822842?mt=8>Go Appstore</a>\"><font color=yellow><b>Invite friends</b></font></a>&nbsp;</td></tr></table></td></tr></table><input type=hidden id=level value=1></body></html>";

						browser.getSettings().setJavaScriptEnabled(true);
						// browser.addJavascriptInterface(new AndroidBridge(),
						// "MYMARKET");
						FrameLayout myLayout = (FrameLayout) findViewById(R.id.mainLayout);
						browser.loadDataWithBaseURL(url, totalSource,
								"text/html", "utf-8", "");
						myLayout.addView(browser);
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				semaphore.release();
			}

			super.onPostExecute(result);
		}

		@Override
		protected void onPreExecute() {

			super.onPreExecute();
		}

		@Override
		protected void onProgressUpdate(Void... unused) {

			super.onProgressUpdate(unused);
		}

		@Override
		protected String doInBackground(String... _params) {
			paramCount = _params.length;

			try {
				// webview request
				if (paramCount == 6) {
					String url = _params[0];
					recvedURL = url;

					int x, y, sx, sy, delekey;
					x = Integer.parseInt(_params[1]);
					y = Integer.parseInt(_params[2]);
					sx = Integer.parseInt(_params[3]);
					sy = Integer.parseInt(_params[4]);
					delekey = Integer.parseInt(_params[5]);

					_x = x;
					_y = y;
					_sx = sx;
					_sy = sy;
					URL ocu = new URL(url);
					HttpURLConnection con = (HttpURLConnection) ocu
							.openConnection(); // URL연결

					con.setRequestMethod("GET");
					con.setDoOutput(false);
					con.connect();

					BufferedReader in = null;
					String inputLine = new String();
					String totalSource = new String();
					InputStream is = con.getInputStream();
					InputStreamReader isr = new InputStreamReader(is, "UTF-8");
					in = new BufferedReader(isr); // URLConnection에서

					// 읽어오기
					while ((inputLine = in.readLine()) != null) {
						// Log.i("JAVA", inputLine);
						totalSource = totalSource + inputLine;
						// System.out.println(inputLine);
					}

					String willSendSource = new String();
					if (totalSource.length() - 500 < 0) {
						willSendSource = totalSource.substring(0);
					} else
						willSendSource = totalSource.substring(totalSource
								.length() - 500);

					mGLView.queueEvent(new LQRunnable(willSendSource, delekey) {
						@Override
						public void run() {
							sendWebViewData(this.totalSource, this.delekey);
						}
					});
					return totalSource;
				}

				// data 만 처리.
				else if (paramCount == 2) {
					String url = _params[0];
					int delekey = Integer.parseInt(_params[1]);

					URL ocu = new URL(url);
					URLConnection con = null;
					BufferedReader in = null;
					String inputLine = new String();
					String totalSource = new String();

					con = ocu.openConnection(); // URL연결
					in = new BufferedReader(new InputStreamReader(
							con.getInputStream(), "UTF-8")); // URLConnection에서
																// 읽어오기
					while ((inputLine = in.readLine()) != null) {
						// Log.i("JAVA", inputLine);
						totalSource = totalSource + inputLine;
						// System.out.println(inputLine);
					}

					// 보낼 데이터는 totalSource 임.

					mGLView.queueEvent(new LQRunnable(totalSource, delekey) {
						@Override
						public void run() {
							sendDataFromURL(this.totalSource, this.delekey);
						}
					});
					// sendDataFromURL(totalSource, delekey);

					return totalSource;
				} else
					return new String("");
			} catch (Exception e) {
				if (paramCount == 6) {
					int delekey = Integer.parseInt(_params[5]);
					mGLView.queueEvent(new LQRunnable("", delekey) {
						@Override
						public void run() {
							sendWebViewData(this.totalSource, this.delekey);
						}
					});
				} else if (paramCount == 2) {
					int delekey = Integer.parseInt(_params[1]);
					mGLView.queueEvent(new LQRunnable("", delekey) {
						@Override
						public void run() {
							sendWebViewData(this.totalSource, this.delekey);
						}
					});
				}
				e.printStackTrace();
			}

			return new String("");
		}
	}

	private native void sendDataFromURL(String datas, int delekey);

	protected void getDataFromURL(final String url, final int delekey) {
		final Context _this = this;
		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				ProcessWebTask pwt = new ProcessWebTask();
				pwt.setContext(_this);
				pwt.execute(url, "" + delekey);
			}
		});

	}

	private native void sendWebViewData(String str, int delekey);

	protected void addWebView(final String url, final int x, final int y,
			final int sx, final int sy, final int delekey) {
		// Log.i("JAVA", "addWebView");
		final Context _this = this;
		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				ProcessWebTask pwt = new ProcessWebTask();
				pwt.setContext(_this);
				pwt.execute(url, "" + x, "" + y, "" + sx, "" + sy, "" + delekey);
			}
		});

	}

	protected void deleteWebView() {
		// Log.i("JAVA", "deleteWebView");
		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					semaphore.acquire();
					if (browser != null) {
						FrameLayout myLayout = (FrameLayout) findViewById(R.id.mainLayout);
						myLayout.removeView(browser);
						browser = null;
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}

	protected void hideWebView() {
		// Log.i("JAVA", "hideWebView");
		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub

				try {
					semaphore.acquire();
					if (browser != null)
						browser.setVisibility(View.GONE);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}

	protected void otherApp(final String packName) {
		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					semaphore.acquire();
					// String packName = KSActivityBase.this.getPackageName();
					Intent market = new Intent(Intent.ACTION_VIEW, Uri
							.parse("market://details?id=" + packName));
					startActivity(market);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}

	protected String getUDID() {
		return deviceId;
	}

	protected String getDeviceInfo() {
		String dinfo = android.os.Build.VERSION.RELEASE + '/';

		dinfo += android.os.Build.MODEL;

		return dinfo;
	}

	protected String getPrice(String _sku) {
		ArrayList<String> skuList = new ArrayList<String>();
		StringTokenizer st = new StringTokenizer(_sku, "/");
		while (st.hasMoreTokens()) {
			skuList.add(st.nextToken());
		}
		
		Bundle querySkus = new Bundle();
		querySkus.putStringArrayList("ITEM_ID_LIST", skuList);
		Bundle skuDetails = null;
		try {
			skuDetails = mService.getSkuDetails(3, getPackageName(), "inapp",
					querySkus);
		}

		catch (RemoteException e) {
			// TODO Auto-generated catch block
			return new String("0");
			// e.printStackTrace();
		}
		JSONObject resultJson = new JSONObject();
		int response = skuDetails.getInt("RESPONSE_CODE");
		if (response == 0) {
			ArrayList<String> responseList = skuDetails
					.getStringArrayList("DETAILS_LIST");

			for (String thisResponse : responseList) {
				JSONObject object = null;
				try {
					object = new JSONObject(thisResponse);
					resultJson.put(object.getString("productId"), object.getString("price"));
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return resultJson.toString();
	}

	protected void buyItem(final String itemID, final int delekey) {
		// SnowFilelog.write("buyItemtry");
		// mHelper.launchPurchaseFlow(this, itemID, RC_REQUEST,
		// mPurchaseFinishedListener);
		// SnowFilelog.write("buyItemtry End");

		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub

				try {
					semaphore.acquire();
					if (KSActivityBase.this.mSetupDone) {
						mHelper.launchPurchaseFlow(KSActivityBase.this, itemID,
								RC_REQUEST, mPurchaseFinishedListener, ""
										+ delekey);
					} else {
						complain("network error, retry it.");

						mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
							public void onIabSetupFinished(IabResult result) {
								if (!result.isSuccess()) {
									// Oh noes, there was a problem.
									mSetupDone = false;
									complain("Network error, in-app billing");
									return;
								}
								KSActivityBase.this.mSetupDone = true;
							}
						});
					}

				} catch (InterruptedException e) {
					return;
					// e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});

		// if( mManagedType != Managed.SUBSCRIPTION )
		// {
		// if( !mBillingService.requestPurchase(mSku, Consts.ITEM_TYPE_INAPP,
		// null) ) // payload = null
		// {
		// showDialog(DIALOG_BILLING_NOT_SUPPORTED_ID);
		// }
		// }
		// ///< 구독
		// else
		// {
		// if( !mBillingService.requestPurchase(mSku,
		// Consts.ITEM_TYPE_SUBSCRIPTION, null) ) // payload = null
		// {
		// showDialog(DIALOG_SUBSCRIPTIONS_NOT_SUPPORTED_ID);
		// }
		// }
	}

	protected void showWebView() {
		// Log.i("JAVA", "showWebView");
		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub

				try {
					semaphore.acquire();
					if (browser != null)
						browser.setVisibility(View.VISIBLE);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	protected void moveWebView(final int x, final int y) {
		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					semaphore.acquire();
					if (browser != null) {
						Size mySize = getFitScreen();
						int left_margin = getLeftMargin();
						int bottom_margin = getBottomMargin();

						double xRatio = mySize.width / 480.;
						double yRatio = mySize.height / 320.;
						// browser.getLayoutParams().width
						// FrameLayout.LayoutParams params = new
						// FrameLayout.LayoutParams( browser.getLayoutParams());
						FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
								browser.getLayoutParams().width, browser
										.getLayoutParams().height, Gravity.TOP);
						params.setMargins((int) (x * xRatio) + left_margin,
								(int) (y * yRatio) + bottom_margin, 0, 0);

						browser.setLayoutParams(params);
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	protected void hideAdmob() {
		// Log.i("JAVA", "hideAdMob in Java");

		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					semaphore.acquire();
					AdView adView = (AdView) findViewById(R.id.adView);
					if (adView != null)
						adView.setVisibility(View.INVISIBLE);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	protected void showAdmob() {
		// Log.i("JAVA", "showAdMob in Java");
		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					semaphore.acquire();
					AdView adView = (AdView) findViewById(R.id.adView);
					if (adView != null)
						adView.setVisibility(View.VISIBLE);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	protected void topAdmob() {

		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					semaphore.acquire();
					AdView adView = (AdView) findViewById(R.id.adView);
					if (adView != null) {
						FrameLayout.LayoutParams pelle = new FrameLayout.LayoutParams(
								FrameLayout.LayoutParams.WRAP_CONTENT,
								FrameLayout.LayoutParams.WRAP_CONTENT,
								Gravity.TOP | Gravity.CENTER_HORIZONTAL);
						pelle.setMargins(0, 0, 0, 0);
						adView.setLayoutParams(pelle);
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	protected void bottomAdmob() {
		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					semaphore.acquire();
					AdView adView = (AdView) findViewById(R.id.adView);
					if (adView != null) {
						FrameLayout.LayoutParams pelle = new FrameLayout.LayoutParams(
								FrameLayout.LayoutParams.WRAP_CONTENT,
								FrameLayout.LayoutParams.WRAP_CONTENT,
								Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL);
						pelle.setMargins(0, 0, 0, 0);
						adView.setLayoutParams(pelle);
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}

	protected void showFacebook() {
		final Context _this = this;

		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					semaphore.acquire();
					etcWebView = new WebView(_this);
					etcWebView.getSettings().setJavaScriptEnabled(true);
					etcWebView.loadUrl("http://www.facebook.com/LitQoo");
					FrameLayout myLayout = (FrameLayout) findViewById(R.id.mainLayout);
					myLayout.addView(etcWebView);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

				// etcWebView.setWebViewClient(new WebViewClient()
				// {
				// @Override
				// public boolean shouldOverrideUrlLoading(WebView view, String
				// url) {
				// view.loadUrl(url);
				// return true;
				// };
				// }
				// );
			}
		});
	}

	protected void showTest() {

		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					semaphore.acquire();

					// SnowFilelog.write(KSActivityBase.this,
					// "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
					// Intent intent = new
					// Intent((Context)KSActivityBase.getActivity(),
					// Dungeons.class);
					// ((Context)KSActivityBase.getActivity()).startActivity(intent);
					// startActivity(intent);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}

	protected void showReview() {
		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					semaphore.acquire();
					if (MARKET_TYPE == MARKET_GOOGLE) {
						String packName = KSActivityBase.this.getPackageName();
						Log.i("", packName);
						Intent market = new Intent(Intent.ACTION_VIEW, Uri
								.parse("market://details?id=" + packName));
						startActivity(market);
					} else if (MARKET_TYPE == MARKET_TSTORE) {
						Intent intent = new Intent();
						intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP); // mandatory
																			// flag
						intent.setClassName("com.skt.skaf.A000Z00040",
								"com.skt.skaf.A000Z00040.A000Z00040");
						intent.setAction("COLLAB_ACTION"); // action
						intent.putExtra("com.skt.skaf.COL.URI",
								"PRODUCT_VIEW/0000297141/0".getBytes()); // user
																			// data
						intent.putExtra("com.skt.skaf.COL.REQUESTER",
								"A000Z00040"); // my App ID
						startActivity(intent);
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	protected void vib() {
		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					semaphore.acquire();
					Vibrator vibe = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
					vibe.vibrate(500);
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}
			}
		});
	}

	protected void showMoreApps() {
		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					semaphore.acquire();
					if (MARKET_TYPE == MARKET_GOOGLE) {
						Intent goToMarket;
						goToMarket = new Intent(Intent.ACTION_VIEW, Uri
								.parse("market://search?q=pub:LitQoo"));
						startActivity(goToMarket);
					} else if (MARKET_TYPE == MARKET_TSTORE) {
						Intent intent = new Intent();
						intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
						intent.setClassName("com.skt.skaf.A000Z00040",
								"com.skt.skaf.A000Z00040.A000Z00040");
						intent.setAction("COLLAB_ACTION");
						intent.putExtra("com.skt.skaf.COL.URI",
								"SEARCH_ACTION/0/LitQoo".getBytes());
						intent.putExtra("com.skt.skaf.COL.REQUESTER",
								"A000Z00040");
						startActivity(intent);
					}
				} catch (InterruptedException e) {
					e.printStackTrace();
				} finally {
					semaphore.release();
				}

			}
		});
	}

	protected String getUTF8String(String _strKey) {
		int resourceID = getResources().getIdentifier(_strKey, "string",
				this.getPackageName());
		// Log.i("tt", "" + resourceID);
		if (resourceID != 0)
			return getResources().getString(resourceID);
		else {
			return _strKey;
		}
	}

	@Override
	public void onReceiveAd(Ad ad) {
		try {
			semaphore.acquire();
			AdView adView = (AdView) this.findViewById(R.id.adView);
			adView.setVisibility(View.INVISIBLE);
			// adView.setVisibility(View.VISIBLE);
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			semaphore.release();
		}

	}

	@Override
	public void onDismissScreen(Ad arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onFailedToReceiveAd(Ad arg0, ErrorCode arg1) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onLeaveApplication(Ad arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void onPresentScreen(Ad arg0) {
		// TODO Auto-generated method stub

	}

}
