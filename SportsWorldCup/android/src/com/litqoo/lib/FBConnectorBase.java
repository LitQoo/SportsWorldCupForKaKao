package com.litqoo.lib;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;

import com.facebook.android.*;
import com.facebook.android.Facebook.DialogListener;
import com.facebook.FacebookDialogException;
import com.facebook.FacebookException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.HttpMethod;
import com.facebook.Request;
import com.facebook.RequestAsyncTask;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.SessionLoginBehavior;
import com.facebook.SessionState;
import com.facebook.Request.Callback;
import com.facebook.model.GraphUser;
import com.facebook.widget.LoginButton;
import com.facebook.widget.WebDialog;
import com.facebook.widget.WebDialog.OnCompleteListener;
import com.litqoo.SportsWorldCupForKaKao.R;


public class FBConnectorBase extends KSActivityBase{


	///////////////facebook /////////////////////////////////////
	//private native void sendFBData(String datas, int delekey);
	//private native void sendFBRequestData(String datas, int delekey);
	private native void sendFBLoginInfo(String datas, int delekey);
	//private native void sendFBRequestResult(int datas,int delekey);
	//private native void sendFBScoreResult(int datas,int delekey);
	//private native void sendFBRemoveRequestResult(String datas,int delekey);


	private native void sendFBSingleData(String datas,int delekey);
	private native void sendFBArrayData(String datas,int delekey);


	//private static Object getActivity(){
	//	return super.getActivity();
	//}

	 String FILENAME = "AndroidSSO_data";
	 private SharedPreferences mPrefs;
	 Facebook facebook=null;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		
		super.onCreate(savedInstanceState);
		
		facebook = new Facebook(getResources().getString(R.string.app_id));
		Log.i("cocos2d-x","strings "+getResources().getString(R.string.app_id));
		
			Session session = Session.getActiveSession();
			if(session == null) {

	    		if(savedInstanceState != null) {
		    	  session = Session.restoreSession(this, null, null, savedInstanceState);
		    	 }
		        if(session== null) {
		        	session = new Session(this);
		        }
		        Session.setActiveSession(session);
		    }

	}
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
        facebook.authorizeCallback(requestCode, resultCode, data);
		// Session.getActiveSession().onActivityResult(this, requestCode, resultCode, data);
	}
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		Session session = Session.getActiveSession();
		Session.saveSession(session, outState);
	}

//	private static Session openActiveSession(Activity activity, boolean allowLoginUI, Session.StatusCallback callback, List<String> permissions, boolean isRead) {
////		Session session = Session.getActiveSession();
////        
////		if (!session.isOpened() && !session.isClosed() && session.getState() != SessionState.OPENING) {
////			Log.i("cocos2d-x", "login openforpublish");
////			Session.OpenRequest open = new Session.OpenRequest(activity).setCallback(callback);
////            List<String> permission = new ArrayList<String>();
////            permission.add("publish_actions");
////            open.setPermissions(permission);
////            session.openForPublish(open);
////        } else {
////        	Log.i("cocos2d-x", "open");
////            Session.openActiveSession(activity, true, callback);
////        }
//		
//		Session.OpenRequest openRequest = new Session.OpenRequest(activity).setCallback(callback);
//		if(permissions.size()>0)openRequest.setPermissions(permissions);
//		Session session = new Session.Builder(activity).build();
//		if (SessionState.CREATED_TOKEN_LOADED.equals(session.getState()) || allowLoginUI) {
//			Session.setActiveSession(session);
//			if(isRead){
//				session.openForRead(openRequest);
//			}else{ 
//				session.openForPublish(openRequest);}
//			return session;
//		}
//		return null;
//	}
	


//	private static final List<String> PERMISSIONS = Arrays.asList("publish_actions");
//	private boolean pendingPublishReauthorization = false;
//	
//	private void handleAnnounce() {
//	    //Log.d(LOG_TAG, "handleAnnounce");
//	    Session session = Session.getActiveSession();
//	    if (session == null || !session.isOpened()) {
//	        return;
//	    }
//
//	    List<String> permissions = session.getPermissions();
//	    if (!permissions.containsAll(PERMISSIONS)) {
//		    if (session != null) {
//		        List<String> publishPermissions = Arrays.asList("publish_actions");
//		        final int REAUTH_ACTIVITY_CODE = 100;
//		        Session.NewPermissionsRequest reauthRequest = new Session.NewPermissionsRequest(this, publishPermissions).setRequestCode(REAUTH_ACTIVITY_CODE);
//		        reauthRequest.setLoginBehavior(SessionLoginBehavior.SUPPRESS_SSO);
//		        session.requestNewPublishPermissions(reauthRequest);
//		    }
//	        return;
//	    }
//	}
//	
	
	
public void fb_myinfo(final int delekey){
		final Session session = Session.getActiveSession();
					if (session.isOpened()) {
						Request.executeMeRequestAsync(session, new Request.GraphUserCallback() {
							// callback after Graph API response with user object
							@Override
							public void onCompleted(final GraphUser user, Response response) {
								if (user != null) {
									Request req = new Request(session, user.getId()+"/scores",null,HttpMethod.GET,  new Callback() {
										@Override
										public void onCompleted(Response response2) {
											JSONObject _scoreinfo = new JSONObject();
											
											try {
												_scoreinfo = response2.getGraphObject().getInnerJSONObject().getJSONArray("data").getJSONObject(0);
											} catch (JSONException e) {
												// TODO Auto-generated catch block
												try {
													_scoreinfo.put("score", 0);
													JSONObject newScoreInfo = new JSONObject();
													newScoreInfo.put("id", user.getId());
													newScoreInfo.put("name", user.getName());
													_scoreinfo.put("user", newScoreInfo);
												} catch (JSONException e1) {
													e1.printStackTrace();
													mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
													return;
												}
												
												//mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBLoginInfo("{\"score\":0,\"user\":{\"id\":\""+user.getId()+"\",\"name\":\""+user.getName()+"\"}}",delekey);}});
												//return;
											}	
											
											String _sendData = _scoreinfo.toString();
											mGLView.queueEvent(new LQRunnable(_sendData, delekey) {@Override public void run() {sendFBLoginInfo(this.totalSource,delekey);}});
											return;
										}
									});

									req.executeAsync();
								}else{
									mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
									return;
								}
							}
						});
					}else if(session.isClosed())
					{
						mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
						return;
					}

}

boolean facebookerror=false;
public void fb_login(final int delekey){
	getHandler().post(new Runnable() {
	@Override
	public void run() {
		mPrefs = getPreferences(MODE_PRIVATE);
		String access_token = mPrefs.getString("access_token", null);
        long expires = mPrefs.getLong("access_expires", 0);
        if(access_token != null) {
        	facebook.setAccessToken(access_token);
        }
        if(expires != 0) {
            facebook.setAccessExpires(expires);
        }
        //facebook.isSessionValid()
		if(facebook.isSessionValid()){
//			try {
//				String result = facebook.request("me");
//			} catch (MalformedURLException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			} catch (IOException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
			Session.setActiveSession(facebook.getSession());
			fb_myinfo(delekey);
		}else{
		//, Facebook.FORCE_DIALOG_AUTH
		int loginmode = 32665;
		if(facebookerror==true){
			loginmode=Facebook.FORCE_DIALOG_AUTH;
		}
		facebook.authorize((Activity)KSActivityBase.getActivity(), new String[]{"user_birthday","publish_actions"},loginmode,
                new DialogListener() {
            @Override
            public void onComplete(Bundle values) {
            	SharedPreferences.Editor editor = mPrefs.edit();
                editor.putString("access_token", facebook.getAccessToken());
                editor.putLong("access_expires", facebook.getAccessExpires());
                editor.commit();
//            	Log.i("cocos2d-x","login oncomplete start me"); 
//            	
//            	Bundle params = new Bundle();
//				params.putString("fields", "id,name");
//				String result = null;
//            	JSONObject json = null;
//				String userName="";
//				String userID="";
//					result = facebook.request("me", params);
//					json = Util.parseJson(result);
//					userName = json.getString("name");
//					userID=json.getString("id");
//					
//					result = facebook.request(userID+"/scores");
//					json = Util.parseJson(result);
//					Log.i("cocos2d-x","result :"+json.toString());
//					
//					JSONObject _scoreinfo = new JSONObject();
//					_scoreinfo = json.getJSONArray("data").getJSONObject(0);
//					
//					String _sendData = _scoreinfo.toString();
					
					Session.setActiveSession(facebook.getSession());
					fb_myinfo(delekey);
//					mGLView.queueEvent(new LQRunnable(_sendData, delekey) {@Override public void run() {sendFBLoginInfo(this.totalSource,delekey);}});
					

            	
            }

            @Override
            public void onFacebookError(FacebookError error) {
				
				if(facebookerror==true){
					mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
				}else{
					facebookerror=true;
					fb_login(delekey);
				}
            	
            }

            @Override
            public void onError(DialogError e) {
				if(facebookerror==true){
					mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
				}else{
					facebookerror=true;
					fb_login(delekey);
				}
            }

            @Override
            public void onCancel() {
            	mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
            	
            }
        });
		}
	}});
        
//        
//		Log.i("cocos2d-x", "fb_login");
//		getHandler().post(new Runnable() {
//			@Override
//			public void run() {
//
//				Log.i("cocos2d-x", "fb_login2");
//				// start Facebook Login
//				openActiveSession((Activity)KSActivityBase.getActivity() , true, new Session.StatusCallback() {
//					// callback when session changes state
//					@Override
//					public void call(final Session session, SessionState state, Exception exception) {
//						Log.i("cocos2d-x", "call");
//						Log.i("cocos2d-x", state.name());
//						Log.i("cocos2d-x", session.toString());
//						String tt = session.getAccessToken();
//						Log.i("cocos2d-x", tt);
//						if (session.isOpened()) {
//							Log.i("cocos2d-x", "session opened");
//							Request.executeMeRequestAsync(session, new Request.GraphUserCallback() {
//								// callback after Graph API response with user object
//								@Override
//								public void onCompleted(final GraphUser user, Response response) {
//									
//									if (user != null) {
//										
//										Request req = new Request(session, user.getId()+"/scores",null,HttpMethod.GET,  new Callback() {
//											@Override
//											public void onCompleted(Response response2) {
//												JSONObject _scoreinfo = new JSONObject();
//												
//												try {
//													_scoreinfo = response2.getGraphObject().getInnerJSONObject().getJSONArray("data").getJSONObject(0);
//												} catch (JSONException e) {
//													// TODO Auto-generated catch block
//													try {
//														_scoreinfo.put("score", 0);
//														JSONObject newScoreInfo = new JSONObject();
//														newScoreInfo.put("id", user.getId());
//														newScoreInfo.put("name", user.getName());
//														_scoreinfo.put("user", newScoreInfo);
//													} catch (JSONException e1) {
//														e1.printStackTrace();
//														mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
//														return;
//													}
//													
//													//mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBLoginInfo("{\"score\":0,\"user\":{\"id\":\""+user.getId()+"\",\"name\":\""+user.getName()+"\"}}",delekey);}});
//													return;
//												}	
//												
//												////////////////////////////////////////////////////////////////////////////////
//												
//												
////												List<String> permissions = session.getPermissions();
////										        if (!isSubsetOf(PERMISSIONS, permissions)) {
////										        	Log.i("cocos2d-x","get publish permission");
////											        List<String> publishPermissions = Arrays.asList("publish_actions");
////											        final int REAUTH_ACTIVITY_CODE = 100;
////											        Session.NewPermissionsRequest reauthRequest = new Session.NewPermissionsRequest((Activity)KSActivityBase.getActivity(), publishPermissions).setRequestCode(REAUTH_ACTIVITY_CODE);
////											        reauthRequest.setLoginBehavior(SessionLoginBehavior.SUPPRESS_SSO);
////											        session.requestNewPublishPermissions(reauthRequest);
////										        }
//												 if (!session.getPermissions().contains("publish_actions")) {
//											            // We need to reauthorize, then complete the action when we get called back.
//													 	Log.i("cocos2d-x","check publishpermission");
//														Session.OpenRequest openRequest = new Session.OpenRequest((Activity)KSActivityBase.getActivity()).setCallback(null);
//														 Log.i("cocos2d-x","check publishpermission2");
//														List<String> publishPermissions = Arrays.asList("publish_actions");
//														 Log.i("cocos2d-x","check publishpermission3");
//														openRequest.setPermissions(publishPermissions);
//														 Log.i("cocos2d-x","check publishpermission4");
//														Session session = new Session.Builder((Activity)KSActivityBase.getActivity()).build();
//														 Log.i("cocos2d-x","check publishpermission5");
//														
//														session.openForPublish(openRequest);
//														 Log.i("cocos2d-x","check publishpermission6");
//											      }
//										        /////////////////////////////////////////////////////////////////////////////
//												String _sendData = _scoreinfo.toString();
//												mGLView.queueEvent(new LQRunnable(_sendData, delekey) {@Override public void run() {sendFBLoginInfo(this.totalSource,delekey);}});
//												return;
//											}
//										});
//
//										req.executeAsync();
//									}else{
//										Log.i("cocos2d-x", "user == null");
//										mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
//										return;
//									}
//								}
//							});
//						}else if(session.isClosed())
//						{
//							Log.i("cocos2d-x", "session.isClosed()");
//							mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
//							return;
//						}
//					}
//				},Arrays.asList("user_birthday"),true);
//			}}); 
	
	}

	public void fb_logout(){

		getHandler().post(new Runnable() {
			@Override
			public void run() {

				Session.getActiveSession().closeAndClearTokenInformation();
				Session.setActiveSession(null);
				Session newSession = new Session(((Context)KSActivityBase.getActivity()));
				Session.setActiveSession(newSession);
			}});
	}

	public void fb_getscores(final String appID, final String userID, final int delekey){
//		getHandler().post(new Runnable() {
//		@Override
//		public void run() {
//		
//		Log.i("cocos2d-x","login oncomplete start me"); 
//    	
//		try {
//			String result = facebook.request(appID+"/scores");
//			JSONObject _scores = Util.parseJson(result);
//			JSONArray scoreArray= _scores.getJSONArray("data");		
//			
//			Bundle friendParam = new Bundle();
//			friendParam.putString("q", "SELECT uid, name FROM user WHERE uid IN (SELECT uid2 FROM friend WHERE uid1 = me()) ORDER BY rand() limit 30");
//			String friendResult = facebook.request("fql", friendParam);
//			JSONObject _friends = Util.parseJson(friendResult);
//			JSONArray friendsArray= _friends.getJSONArray("data");	
//			
//				for(int i=0;i<friendsArray.length();i++){
//					JSONObject friend = friendsArray.getJSONObject(i);
//					String fID = friend.getString("uid");
//					String fName = friend.getString("name");
//					int check=0;
//					for(int j=0;j<scoreArray.length();j++){
//						JSONObject scoreInfo = scoreArray.getJSONObject(j);
//						String sID = scoreInfo.getJSONObject("user").getString("id");
//						if(fID.equals(sID)){
//							check=1;
//							break;
//						}
//					}
//
//					if(check==0){
//						JSONObject newScore = new JSONObject();
//						newScore.put("score", -1);
//						JSONObject newScoreInfo = new JSONObject();
//						newScoreInfo.put("id", fID);
//						newScoreInfo.put("name", fName);
//						newScore.put("user", newScoreInfo);
//
//						scoreArray.put(newScore);
//					}
//				}
//
//
//
//			String _data = scoreArray.toString();
//			////////////////////////////
//			mGLView.queueEvent(new LQRunnable(_data, delekey) {
//				@Override
//				public void run() {
//					try {
//						sendFBArrayData("start", this.delekey);
//						JSONArray friendsArray = new JSONArray(this.totalSource);
//						for(int i=0;i<friendsArray.length();i++){
//							JSONObject friend = friendsArray.getJSONObject(i);;
//							String _sendBuff = friend.toString();
//							Log.i("cocos2d-x", "pwt, sendFBDataStart "+_sendBuff +" "+delekey);
//							sendFBArrayData(_sendBuff, this.delekey);
//							Log.i("cocos2d-x", "pwt, sendFBDataEnd "+_sendBuff +" "+delekey);
//
//						}
//						sendFBArrayData("end", this.delekey);
//
//					} catch (JSONException e) {
//						// TODO Auto-generated catch block
////						e.printStackTrace();
//						mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
//						return;
//					}
//
//				}
//			});
//			
//		} catch (MalformedURLException e) {
//			// TODO Auto-generated catch block
//			mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
//			e.printStackTrace();
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
//			e.printStackTrace();
//		}catch (FacebookError e) {
//			// TODO Auto-generated catch block
//			mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
//			e.printStackTrace();
//		} catch (JSONException e) {
//			// TODO Auto-generated catch block
//			mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
//			e.printStackTrace();
//		}
//		
//		}});
		final Session session = Session.getActiveSession();
		getHandler().post(new Runnable() {
			@Override
			public void run() {
				/////////////////////////////////////////////////////
				if (session.isOpened()) {

					Bundle params = new Bundle();
					params.putString("fields", "score,user");
					Request req = new Request(session, appID+"/scores",params,HttpMethod.GET,  new Callback() {
						@Override
						public void onCompleted(Response response) {
							
							JSONArray _scoreArray;
							
							try {
								if(response.getGraphObject() == null)
								{
									throw (new JSONException(""));
								}
								else
								{
									_scoreArray = response.getGraphObject().getInnerJSONObject().getJSONArray("data");								
								}
								
							} catch (JSONException e) {
								// TODO Auto-generated catch block
								_scoreArray = null;
								mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":4}",delekey);}});
//								e.printStackTrace();
								return;
							}		            	  


							final JSONArray scoreArray=_scoreArray;
							////////////////////////////////////////
							Bundle params = new Bundle();

							params.putString("q", "SELECT uid, name FROM user WHERE uid IN (SELECT uid2 FROM friend WHERE uid1 = me()) ORDER BY rand() limit 30");
							Request req = new Request(session, "/fql",params,HttpMethod.GET,  new Callback() {
								@Override
								public void onCompleted(Response response) {
									try{
										JSONArray friendsArray = response.getGraphObject().getInnerJSONObject().getJSONArray("data");
										for(int i=0;i<friendsArray.length();i++){
											JSONObject friend = friendsArray.getJSONObject(i);
											String fID = friend.getString("uid");
											String fName = friend.getString("name");
											int check=0;
											for(int j=0;j<scoreArray.length();j++){
												JSONObject scoreInfo = scoreArray.getJSONObject(j);
												String sID = scoreInfo.getJSONObject("user").getString("id");
												if(fID.equals(sID)){
													check=1;
													break;
												}
											}

											if(check==0){
												JSONObject newScore = new JSONObject();
												newScore.put("score", -1);
												JSONObject newScoreInfo = new JSONObject();
												newScoreInfo.put("id", fID);
												newScoreInfo.put("name", fName);
												newScore.put("user", newScoreInfo);

												scoreArray.put(newScore);
											}
										}


									}catch(JSONException e){
//										e.printStackTrace();
										mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":4}",delekey);}});
										return;
									}

									String _data = scoreArray.toString();
									////////////////////////////
									mGLView.queueEvent(new LQRunnable(_data, delekey) {
										@Override
										public void run() {
											try {
												sendFBArrayData("start", this.delekey);
												JSONArray friendsArray = new JSONArray(this.totalSource);
												for(int i=0;i<friendsArray.length();i++){
													JSONObject friend = friendsArray.getJSONObject(i);;
													String _sendBuff = friend.toString();
													sendFBArrayData(_sendBuff, this.delekey);

												}
												sendFBArrayData("end", this.delekey);

											} catch (JSONException e) {
												// TODO Auto-generated catch block
//												e.printStackTrace();
												mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
												return;
											}

										}
									});
									/////////////////////////////
								}
							});
							RequestAsyncTask task = new RequestAsyncTask(req);
							task.execute();

							////////////////////////////////////////

						}
					});

					RequestAsyncTask task = new RequestAsyncTask(req);
					task.execute();
				}else{
					mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
					return;
				}

				/////////////////////////////////////////////////////	
			}
		});

	}

	private boolean isSubsetOf(Collection<String> subset, Collection<String> superset) {
	    for (String string : subset) {
	        if (!superset.contains(string)) {
	            return false;
	        }
	    }
	    return true;
	}
	
	
	public void fb_sendScore(final String uID,final int pScore,final int delekey){
		final Session session = Session.getActiveSession();
		getHandler().post(new Runnable() {
			@Override
			public void run() {	  
				if(session.isOpened()){
					Request req = new Request(session, uID+"/scores",null,HttpMethod.GET,  new Callback() {
						@Override
						public void onCompleted(Response response) {    
							int nowScore=0;
							try {
								JSONArray myscore = response.getGraphObject().getInnerJSONObject().getJSONArray("data");
								nowScore=myscore.getJSONObject(0).getInt("score");

							} catch (JSONException e) {
								nowScore=0;
//								// TODO Auto-generated catch block
							}

							if(nowScore<pScore){
								Bundle params = new Bundle();
								params.putString("score", Integer.toString(pScore));
								Request req = new Request(Session.getActiveSession(), uID+"/scores",params,HttpMethod.POST,  new Callback() {
									@Override
									public void onCompleted(Response response) {
										mGLView.queueEvent(new LQRunnable("", delekey) {
											@Override
											public void run() {
												sendFBSingleData("{\"error\":0}",delekey);
											}
										});
									}
								});

								RequestAsyncTask task = new RequestAsyncTask(req);
								task.execute();
							}else{
								mGLView.queueEvent(new LQRunnable("", delekey) {
									@Override
									public void run() {
										sendFBSingleData("{\"error\":0}",delekey);
									}
								});
							}
						}

					});

					RequestAsyncTask task = new RequestAsyncTask(req);
					task.execute();
				}else{
					mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
					return;
				}
			}});

	}


	public void fb_getrequest(final String uid,final int delekey){
		final Session session = Session.getActiveSession();
		getHandler().post(new Runnable() {
			@Override
			public void run() {
				/////////////////////////////////////////////////////
				if (session.isOpened()) {
					Bundle params = new Bundle();
					params.putString("fields", "from,data,message,id,created_time");
					Request req = new Request(session, uid+"/apprequests",params,HttpMethod.GET,  new Callback() {
						@Override
						public void onCompleted(Response response) {

							String _score;
							try {
								if(response.getGraphObject() == null)
								{
									throw(new JSONException(""));
								}
								else
								{
									JSONObject object = response.getGraphObject().getInnerJSONObject();
									_score = object.getJSONArray("data").toString();
								}
								

							} catch (JSONException e) {
								// TODO Auto-generated catch block
								_score="";
//								e.printStackTrace();
								mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
								return;
							}

							mGLView.queueEvent(new LQRunnable(_score, delekey) {
								@Override
								public void run() {
									try {
										sendFBArrayData("start", this.delekey);
										JSONArray friendsArray = new JSONArray(this.totalSource);
										for(int i=0;i<friendsArray.length();i++){
											JSONObject friend = friendsArray.getJSONObject(i);
											String _sendBuff = friend.toString();
											sendFBArrayData(_sendBuff, this.delekey);
										}
										sendFBArrayData("end", this.delekey);

									} catch (JSONException e) {
										// TODO Auto-generated catch block
//										e.printStackTrace();
										mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":5}",delekey);}});
										return;
									}

								}
							});
						}
					});
					RequestAsyncTask task = new RequestAsyncTask(req);
					task.execute();
				}else{
					mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
					return;
				}
			}});
	}

	public void fb_removeRequest(final String rID, final int delekey){
		final Session session = Session.getActiveSession();

		getHandler().post(new Runnable() {
			@Override
			public void run() {
				/////////////////////////////////////////////////////
				if (session.isOpened()) {
					Bundle params = new Bundle();
					Request req = new Request(session, ""+rID,params,HttpMethod.DELETE,  new Callback() {
						@Override
						public void onCompleted(Response response) {
							mGLView.queueEvent(new LQRunnable("", delekey) {
								@Override
								public void run() {
									sendFBSingleData("{\"error\":0,\"id\":\""+rID+"\""+"}",delekey);
								}
							});
						}
					});

					
					RequestAsyncTask task = new RequestAsyncTask(req);
					task.execute();
				}else{
					mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3,\"id\":\""+rID+"\""+"}",delekey);}});
					return;
				}
			}
		});
	}

	public void fb_sendRequest(final String pID, final String pMsg, final String pData, final int delekey){

		final Session session = Session.getActiveSession();

		getHandler().post(new Runnable() {
			@Override
			public void run() {
				/////////////////////////////////////////////////////�담�
				if (session.isOpened()) {
					Bundle params = new Bundle();
					params.putString("message", pMsg);
					params.putString("to", pID);
					params.putString("data", pData);
				
					WebDialog requestsDialog = (
							new WebDialog.RequestsDialogBuilder(((Context)KSActivityBase.getActivity()),
									session,
									params))
									.setOnCompleteListener(new OnCompleteListener() {

										@Override
										public void onComplete(Bundle values,
												FacebookException error) {
											if (error != null) {
												if (error instanceof FacebookOperationCanceledException) {
													mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":2}",delekey);}});
												} else {
													mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
												}
											} else {
												final String requestId = values.getString("request");
												if (requestId != null) {
													mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":0,\"id\":\""+pID+"\"}",delekey);}});
												} else {
													mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":2}",delekey);}});
												}
											}   
										}

									})
									.build();
					requestsDialog.show();

				}else{
					mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
				}
			}
		});
	}

	public void fb_sendInvite(final String fID,final String pAppName,final String pInviteMsg,final String pLink,final String pIconUrl, final int delekey){
//		Log.i("cocos2d-x","invite");
//		  Bundle params = new Bundle();
//		    params.putString("message", "my message here");
//		    facebook.dialog((Context)KSActivityBase.getActivity(), "feed", params, new DialogListener() {
//		        @Override
//		        public void onFacebookError(FacebookError e) {
//		        	Log.i("cocos2d-x","error "+e.toString());
//		        }
//
//		        @Override
//		        public void onError(DialogError e) {
//		        	Log.i("cocos2d-x","error "+e.toString());
//		        }
//
//		        @Override
//		        public void onComplete(Bundle values) {
//		        	Log.i("cocos2d-x","complete");
//		        }
//
//		        @Override
//		        public void onCancel() {
//		        	Log.i("cocos2d-x","cancel");
//		        }
//		    });
		
		final Session session = Session.getActiveSession();
		getHandler().post(new Runnable() {
			@Override
			public void run() {
				/////////////////////////////////////////////////////
				if (session.isOpened()) {
					Bundle params = new Bundle();
					params.putString("to", fID);
					params.putString("name", pAppName);
					params.putString("caption","IOS & Android Game");
					params.putString("description",pInviteMsg);
					params.putString("link",pLink);
					params.putString("picture",pIconUrl);

					WebDialog requestsDialog = (
							new WebDialog.FeedDialogBuilder(((Context)KSActivityBase.getActivity()),
									session,
									params))
									.setOnCompleteListener(new OnCompleteListener() {

										@Override
										public void onComplete(Bundle values,
												FacebookException error) {
											if(values != null)
												Log.i("aasd", values.toString());
											else
											{
												mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":2}",delekey);}});
												return;
											}
											if (error != null) {
												if (error instanceof FacebookOperationCanceledException) {
													mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":2}",delekey);}});
												} else {
													mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
												}
											} else {
												final String requestId = values.getString("post_id");
												if (requestId != null) {
													mGLView.queueEvent(new LQRunnable(null, delekey) {
														@Override public void run() 
														{
															sendFBSingleData("{\"error\":0,\"id\":\""+fID+"\"}",delekey);
														}
													});
												} else {
													mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":2}",delekey);}});
												}
											}   
										}

									})
									.build();
					requestsDialog.show();

				}else{
					mGLView.queueEvent(new LQRunnable(null, delekey) {@Override public void run() {sendFBSingleData("{\"error\":3}",delekey);}});
				}
			}
		});
	}


	///////////////facebook end/////////////////////////////////

}
