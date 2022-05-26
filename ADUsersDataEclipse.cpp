#include <objbase.h>
#include <wchar.h>
#include <activeds.h>
#include <Windows.h>
//#define _WIN32_WINNT 0x0500
//------------------------------>PROGRAM TO PRINT ALL USER DETAILS AND IT ALSO INCLUDES OPTION FOR ACCOUNT UNLOCK IF LOCKED(BY GETTING USER INPUT)<-----------------
#include "atltime.h"
#include <sddl.h>
#include <iostream>
#include <string>
#include <string>
#include <comutil.h>
#include<vector>
#include<algorithm>
//including JNI headers
#include "jni.h"
#include "com_ActiveDirectory_GetADdata.h"
using namespace std;

extern int arr[5] = { 0 };

int IS_BUFFER_ENOUGH(UINT maxAlloc, LPWSTR pszTarget, LPCWSTR pszSource, int toCopy = -1);

int calculatedifference(SYSTEMTIME expdate);

char* convertDatetoString(VARIANT varDate2);
//JNIEXPORT jobject JNICALL Java_ADUsersDetail_GetData(JNIEnv*, jobject, jstring);
JNIEXPORT jobject JNICALL Java_com_ActiveDirectory_GetADdata_GetData(JNIEnv *env, jobject GetADdata, jstring jpath)
{
	//JNI INITILAZATIONS
	jclass java_util_ArrayList;
	jmethodID java_util_ArrayList_;
	jmethodID java_util_ArrayList_size;
	jmethodID java_util_ArrayList_get;
	jmethodID java_util_ArrayList_add;
	java_util_ArrayList = static_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));
	java_util_ArrayList_ = env->GetMethodID(java_util_ArrayList, "<init>", "(I)V");
	java_util_ArrayList_size = env->GetMethodID(java_util_ArrayList, "size", "()I");
	java_util_ArrayList_get = env->GetMethodID(java_util_ArrayList, "get", "(I)Ljava/lang/Object;");
	java_util_ArrayList_add = env->GetMethodID(java_util_ArrayList, "add", "(Ljava/lang/Object;)Z");
	jclass clazz = (*env).FindClass("java/util/ArrayList");
	jobject obj = (*env).NewObject(clazz, (*env).GetMethodID(clazz, "<init>", "()V"));
	jobject result;
	string data;
	vector<string> list1;
	const string content[20];
	int maxAlloc = MAX_PATH * 2;
	LPOLESTR pszBuffer = new OLECHAR[maxAlloc];
	wcscpy_s(pszBuffer, maxAlloc, L"");
	BOOL bReturnVerbose = TRUE;
	wprintf(L"\nFinding all user objects...\n\n");

	//authentication to user
	CoInitialize(NULL);
	HRESULT hr = S_OK;
	//Get rootDSE and the current user's domain container DN.
	IADs* pObject = NULL;
	IDirectorySearch* pContainerToSearch = NULL;
	LPOLESTR szPath = new OLECHAR[MAX_PATH];
	VARIANT var;
	hr = ADsOpenObject(L"LDAP://rootDSE",
		NULL,
		NULL,
		ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
		IID_IADs,
		(void**)&pObject);
	if (FAILED(hr))
	{
		wprintf(L"Could not execute query. Could not bind to LDAP://rootDSE.\n");
		if (pObject)
			pObject->Release();
		delete[] pszBuffer;
		delete[] szPath;
		CoUninitialize();
		data = "Could not execute query.Could not bind to LDAP ://rootDSE.";
	}
	if (SUCCEEDED(hr))
	{
		hr = pObject->Get((BSTR)L"defaultNamingContext", &var);
		if (SUCCEEDED(hr))
		{
			//Build path to the domain container.
			wcscpy_s(szPath, MAX_PATH, L"LDAP://");
			if (IS_BUFFER_ENOUGH(MAX_PATH, szPath, var.bstrVal) > 0)
			{
				wcscat_s(szPath, MAX_PATH, var.bstrVal);
			}
			else
			{
				wprintf(L"Buffer is too small for the domain DN");
				delete[] pszBuffer;
				delete[] szPath;
				CoUninitialize();
				data="Buffer is too small for the domain DN";
				
			}
			hr = ADsOpenObject(szPath,
				NULL,
				NULL,
				ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
				IID_IDirectorySearch,
				(void**)&pContainerToSearch);

			if (SUCCEEDED(hr))
			{
				//additional addon
				if (!pContainerToSearch)
				{
					
					data = "could not fetch data.Out of Memory";
				}
				//Create search filter
				LPOLESTR pszSearchFilter = new OLECHAR[MAX_PATH * 2];
				if (!pszSearchFilter)
				{
					//return E_OUTOFMEMORY;
					data = "Could not Fetch data.Out of Memory";
				}
				wchar_t szFormat[] = L"(&(objectClass=user)(objectCategory=person)%s)";// common query which includes all users
				if (IS_BUFFER_ENOUGH(MAX_PATH * 2, szFormat, pszBuffer) > 0)
				{
					//Add the filter.
					swprintf_s(pszSearchFilter, MAX_PATH * 2, szFormat, pszBuffer);
				}
				else
				{
					wprintf(L"The filter is too large for buffer, aborting...");
					delete[] pszSearchFilter;
					data = "The filter is too large for buffer, aborting...";
				}
				//Specify subtree search
				ADS_SEARCHPREF_INFO SearchPrefs;
				SearchPrefs.dwSearchPref = ADS_SEARCHPREF_SEARCH_SCOPE;
				SearchPrefs.vValue.dwType = ADSTYPE_INTEGER;
				SearchPrefs.vValue.Integer = ADS_SCOPE_SUBTREE;
				DWORD dwNumPrefs = 1;

				// COL for iterations
				LPOLESTR pszColumn = NULL;
				ADS_SEARCH_COLUMN col;
				HRESULT hr;

				// Interface Pointers
				IADs* pObj = NULL;
				IADs* pIADs = NULL;

				// Handle used for searching
				ADS_SEARCH_HANDLE hSearch = NULL;

				// Set the search preference
				hr = pContainerToSearch->SetSearchPreference(&SearchPrefs, dwNumPrefs);
				if (FAILED(hr))
				{
					delete[] pszSearchFilter;
					//return hr; should change return type
				}

				SYSTEMTIME systemtime;
				DATE date;

				LPOLESTR szName = new OLECHAR[MAX_PATH];
				LPOLESTR szDN = new OLECHAR[MAX_PATH];
				if (!szName || !szDN)
				{
					delete[] pszSearchFilter;
					if (szDN)
						delete[] szDN;
					if (szName)
						delete[] szName;

					//return E_OUTOFMEMORY;
				}

				int iCount = 0;
				int innercolumn = 0;
				DWORD x = 0L;
				hr = pContainerToSearch->ExecuteSearch(pszSearchFilter,
					NULL,
					-1L,
					&hSearch);
				if (SUCCEEDED(hr))
				{
					// Call IDirectorySearch::GetNextRow() to retrieve the next row 
					//of data
					hr = pContainerToSearch->GetFirstRow(hSearch);
					if (SUCCEEDED(hr))
					{
						int outeriter = 0, inneriter = 0;

						while (hr != S_ADS_NOMORE_ROWS)
						{
							int iter = 0;
							vector<string> list2;
							//list2.push_back("[+] Personal Data");
							cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Printing User Details %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << "\n";
							//Keep track of count.
							iCount++;
							if (bReturnVerbose)
								wprintf(L"##########################################################################\n");
							// loop through the array of passed column names,
							// print the data for each column

							while (pContainerToSearch->GetNextColumnName(hSearch, &pszColumn) != S_ADS_NOMORE_COLUMNS)
							{
								hr = pContainerToSearch->GetColumn(hSearch, pszColumn, &col);
								if (SUCCEEDED(hr))
								{
									// Print the data for the column and free the column
									if (bReturnVerbose)
									{
										// Get the data for this column
										//wprintf(L"%s\n", col.pszAttrName);
										switch (col.dwADsType)
										{
										case ADSTYPE_CASE_EXACT_STRING:
										case ADSTYPE_CASE_IGNORE_STRING:
										case ADSTYPE_PRINTABLE_STRING:
										case ADSTYPE_NUMERIC_STRING:
										case ADSTYPE_TYPEDNAME:
										case ADSTYPE_FAXNUMBER:
										case ADSTYPE_PATH:
										case ADSTYPE_OBJECT_CLASS:

											for (x = 0; x < col.dwNumValues; x++)
											{
												iter = iter + 1;
												wprintf(L"  %s\r\n", col.pADsValues[x].CaseIgnoreString);
												char* st = _com_util::ConvertBSTRToString(col.pADsValues[x].CaseIgnoreString);
												cout << "[+] The name conversion after changing to string is : " << st << "\n";
												if (iter == 5)
												{
													list2.push_back(st);
												}
												if (wcscmp(L"ADsPath", col.pszAttrName) == 0)
												{

													IADsUser* pUser;
													SYSTEMTIME ExpirationDate;


													hr = ADsGetObject(col.pADsValues[x].CaseIgnoreString, IID_IADsUser, (void**)&pUser);
													if (SUCCEEDED(hr))
													{
														//password expiration date
														DATE expirationDate;
														VARIANT varDate;
														hr = pUser->get_PasswordExpirationDate(&expirationDate);
														SYSTEMTIME PasswordExpirationDate;
														VariantTimeToSystemTime(expirationDate, &PasswordExpirationDate);
														list2.push_back("[+] Password expire settings:");
														if (SUCCEEDED(hr))
														{
															varDate.vt = VT_DATE;
															varDate.date = expirationDate;
															VariantChangeType(&varDate, &varDate, VARIANT_NOVALUEPROP, VT_BSTR);

															VariantTimeToSystemTime(expirationDate, &ExpirationDate);
															wprintf(L"[+] Password expire settings:\r\n");

															if (ExpirationDate.wYear == 1970) {
																wprintf(L"    password never expires\r\n");
																list2.push_back("password never expires");
															}
															else if (ExpirationDate.wYear == 1899)
															{
																wprintf(L"   User must change password at next login\r\n");
																list2.push_back("User must change password at next login");
															}
															else if (ExpirationDate.wYear > 1970)
															{
																calculatedifference(PasswordExpirationDate);
																int* q = arr;
																if (q[0] < 0 || q[1] < 0 || q[2] < 0)
																{
																	string st;
																	if (q[0] == 0 && q[2] != 0)
																	{
																		std::cout << "    Password Expired " << 0 << " days " << 0 - q[2] << " hours " << 0 - q[1] << " minutes ago" << "\n";
																		list2.push_back("    Password Expired " + to_string(0) + " days " + to_string(0 - q[2]) + " hours " + to_string(0 - q[1]) + " minutes ago");
																		wprintf(L"    password expires at(UTC): %02d-%02d-%02d %02d:%02d:%02d\r\n", ExpirationDate.wDay, ExpirationDate.wMonth, ExpirationDate.wYear, ExpirationDate.wHour, ExpirationDate.wMinute, ExpirationDate.wSecond);
																		std::cout << "    PasswordExpirationDate : " << "\t";
																	}
																	else if (q[0] != 0 && q[2] == 0)
																	{
																		std::cout << "    Password Expired " << 0 - q[0] << " days " << 0 << " hours " << 0 - q[1] << " minutes ago" << "\n";
																		list2.push_back("    Password Expired " + to_string(0 - q[0]) + " days " + to_string(0) + " hours " + to_string(0 - q[1]) + " minutes ago");
																		wprintf(L"    password expires at(UTC): %02d-%02d-%02d %02d:%02d:%02d\r\n", ExpirationDate.wDay, ExpirationDate.wMonth, ExpirationDate.wYear, ExpirationDate.wHour, ExpirationDate.wMinute, ExpirationDate.wSecond);
																		std::cout << "    PasswordExpirationDate : " << "\t";
																	}
																	else if (q[0] == 0 && q[2] == 0)
																	{
																		std::cout << "    Password Expired " << 0 << " days " << 0 << " hours " << 0 - q[1] << " minutes ago" << "\n";
																		list2.push_back("    Password Expired " + to_string(0) + " days " + to_string(0) + " hours " + to_string(0 - q[1]) + " minutes ago");
																		wprintf(L"    password expires at(UTC): %02d-%02d-%02d %02d:%02d:%02d\r\n", ExpirationDate.wDay, ExpirationDate.wMonth, ExpirationDate.wYear, ExpirationDate.wHour, ExpirationDate.wMinute, ExpirationDate.wSecond);
																		std::cout << "    PasswordExpirationDate : " << "\t";
																	}
																	else
																	{
																		std::cout << "    Password Expired " << 0 - q[0] << " days " << 0 - q[2] << " hours " << 0 - q[1] << " minutes ago" << "\n";
																		list2.push_back("    Password Expired " + to_string(0 - q[0]) + " days " + to_string(0 - q[2]) + " hours " + to_string(0 - q[1]) + " minutes ago");
																		wprintf(L"    password expires at(UTC): %02d-%02d-%02d %02d:%02d:%02d\r\n", ExpirationDate.wDay, ExpirationDate.wMonth, ExpirationDate.wYear, ExpirationDate.wHour, ExpirationDate.wMinute, ExpirationDate.wSecond);
																		std::cout << "    PasswordExpirationDate : " << "\t";

																	}
																	wprintf(L"  %s\r\n", varDate.bstrVal);
																	list2.push_back(convertDatetoString(varDate));
																	VariantClear(&varDate);
																	/*std::cout << "Do you want to  Reset Password Expiry Date [(Y||N)]: ";
																	cin >> st;
																	if (st == "y" || st == "Y")
																	{
																		CComBSTR sbstrProp;
																		CComVariant svar;
																		sbstrProp = "pwdLastSet";
																		svar = -1;
																		hr = pUser->Put(sbstrProp, svar);
																	}*/
																}
																else {
																	std::cout << "    Password will expire in " << q[0] << " days " << q[2] << " hours " << q[1] << " minutes" << "\n";
																	wprintf(L"    password expires at(UTC): %02d-%02d-%02d %02d:%02d:%02d\r\n", ExpirationDate.wDay, ExpirationDate.wMonth, ExpirationDate.wYear, ExpirationDate.wHour, ExpirationDate.wMinute, ExpirationDate.wSecond);
																	std::cout << "    PasswordExpirationDate : " << "\t";
																	wprintf(L"  %s\r\n", varDate.bstrVal);
																	list2.push_back("Password will expire in "+ to_string(q[0])+ " days "+to_string(q[2])+" hours "+to_string(q[1])+ " minutes");
																	list2.push_back(convertDatetoString(varDate));
																	VariantClear(&varDate);
																}
															}
														}
														// Account enabled or disabled
														list2.push_back("[+] Account Enabled/Disabled : ");
														wprintf(L"[+] Account options:\r\n");
														VARIANT_BOOL pfAccountDisabled;
														hr = pUser->get_AccountDisabled(&pfAccountDisabled);
														if (SUCCEEDED(hr))
															if (pfAccountDisabled != 0) {
																wprintf(L"    account disabled\r\n");
																list2.push_back("account disabled");
															}
															else if (pfAccountDisabled == 0) {
																wprintf(L"    account enabled\r\n");
																list2.push_back("account enabled");
															}

														//bad logon count
														list2.push_back("[+] BadLogonCount : ");
														LONG get_BadLogincount;
														hr = pUser->get_BadLoginCount(&get_BadLogincount);
														std::cout << "[+] BadLogoncount : " << get_BadLogincount << "\n";
														list2.push_back(to_string(get_BadLogincount));

														//passowrd last changed
														list2.push_back("[+] PasswordLastChanged : ");
														DATE pswdlastchange;
														VARIANT varDate1;
														hr = pUser->get_PasswordLastChanged(&pswdlastchange);
														if (SUCCEEDED(hr))
														{
															varDate1.vt = VT_DATE;
															varDate1.date = pswdlastchange;
															VariantChangeType(&varDate1, &varDate1, VARIANT_NOVALUEPROP, VT_BSTR);
															std::cout << "[+] PasswordLastChanged : " << "\t";
															wprintf(L"  %s\r\n", varDate1.bstrVal);
															list2.push_back((string)convertDatetoString(varDate1));
															VariantClear(&varDate1);
														}
														else
														{
															cout << "Haven't changed the password from the created date"<<"\n";
															list2.push_back("Haven't changed the password from the created date");

														}
														//ACCOUNT EXPIRATION DATE
														list2.push_back("[+] Account Expiry Details : ");
														DATE accexpirationDate;
														hr = pUser->get_AccountExpirationDate(&accexpirationDate);
														if (SUCCEEDED(hr))
														{
															// additional details
															varDate.vt = VT_DATE;
															varDate.date = accexpirationDate;
															VariantChangeType(&varDate, &varDate, VARIANT_NOVALUEPROP, VT_BSTR);
															SYSTEMTIME ExpirationDate;
															VariantTimeToSystemTime(accexpirationDate, &ExpirationDate);
															if (ExpirationDate.wYear == 1970)
															{
																std::cout << "   Account Never Expires " << "\n";
																list2.push_back("Account Never Expires");
															}
															else
															{
																calculatedifference(ExpirationDate);
																int* p = arr;
																if (p[1] < 0 || p[2] < 0 || p[3] < 0)
																{
																	if (p[0] == 0 && p[2] != 0)
																	{
																		std::cout << "    Account Expired " << 0 << " days " << 0 - p[2] << " hours " << 0 - p[1] << " minutes ago" << "\n";
																		list2.push_back(" Account Expired " + to_string(0) + " days " + to_string(0 - p[2]) + " hours " + to_string(0 - p[1]) + " minutes ago");
																		std::cout << "    AccountExpirationDate : " << "\t";
																	}
																	else if (p[0] != 0 && p[2] == 0)
																	{
																		std::cout << "    Account Expired " << 0 - p[0] << " days " << 0 << " hours " << 0 - p[1] << " minutes ago" << "\n";
																		list2.push_back("Account Expired " + to_string(0 - p[0]) + " days " + to_string(0) + " hours " + to_string(0 - p[1]) + " minutes ago");
																		std::cout << "    AccountExpirationDate : " << "\t";
																	}
																	else if (p[0] == 0 && p[2] == 0)
																	{
																		std::cout << "    Account Expired " << 0 << " days " << 0 << " hours " << 0 - p[1] << " minutes ago" << "\n";
																		list2.push_back("Account Expired " + to_string(0) + " days " + to_string(0) + " hours " + to_string(0 - p[1]) + " minutes ago");
																		std::cout << "    AccountExpirationDate : " << "\t";
																	}
																	else
																	{
																		std::cout << "    Account Expired " << 0 - p[0] << " days " << 0 - p[2] << " hours " << 0 - p[1] << " minutes ago" << "\n";
																		list2.push_back("Account Expired " + to_string(0 - p[0]) + " days " + to_string(0 - p[2]) + " hours " + to_string(0 - p[1]) + " minutes ago");
																		std::cout << "    AccountExpirationDate : " << "\t";
																	}
																	//wprintf(L"  %s\r\n", varDate.bstrVal);
																	//list2.push_back(convertDatetoString(varDate));
																	//std::cout << "Account Expired " << 0-p[0]<< " days " << 0-p[2] << " hours " << 0-p[1] << " minutes ago"<<"\n";
																}
																else
																{
																	std::cout << "    AccountExpirationDate : " << "\t";
																	wprintf(L"  %s\r\n", varDate.bstrVal);
																	list2.push_back(convertDatetoString(varDate));
																}
															}
															VariantClear(&varDate);
														}

														//last failed login
													    list2.push_back("[+] LastFailedLogin : ");
														DATE lastfailedlogin;
														VARIANT varDate3;
														hr = pUser->get_LastFailedLogin(&lastfailedlogin);
														//std::cout << "the time value in double is : " << failedlogin << "\n";
														if (SUCCEEDED(hr))
														{
															varDate3.vt = VT_DATE;
															varDate3.date = lastfailedlogin;
															VariantChangeType(&varDate3, &varDate3, VARIANT_NOVALUEPROP, VT_BSTR);
															std::cout << "    LastFailedLogin : " << "\t";
															wprintf(L"  %s\r\n", varDate3.bstrVal);
															list2.push_back(convertDatetoString(varDate3));
															VariantClear(&varDate3);
														}
														else
														{
															std::cout << "[+] LastFailedLogin : " << "\t";
															std::cout << "Haven't Logined After account created" << "\n";
															list2.push_back("Haven't Logined After account created");
														}

														//account locked or not
														list2.push_back("[+] LOCK_STATUS :");
														VARIANT_BOOL isaccountlocked;
														hr = pUser->get_IsAccountLocked(&isaccountlocked);
														if (isaccountlocked == 0)
														{
															std::cout << "[+] LOCK_STATUS : NOT LOCKED" << "\n";
															list2.push_back("Not locked");
															//data[outeriter][inneriter++] = "Not Locked";
														}
														else
														{
															std::cout << "[+] LOCK_STATUS : LOCKED" << "\n";
															list2.push_back("locked");
															/*string stat;
															std::cout << "    Do you want to unlock the account ([Y/N]) : ";
															std::cin >> stat;
															if (stat == "y" || stat == "Y")
															{
																hr = pUser->put_IsAccountLocked(VARIANT_FALSE);
																hr = pUser->SetInfo();
																std::cout << "---->Account unlocked<-----";
															}
															else
															{
																std::cout << "------>Unlocked Failed<------";
															}*/
														}
														pUser->Release();
													}
												}
											}
										}
									}
									pContainerToSearch->FreeColumn(&col);
								}
								FreeADsMem(pszColumn);
							}
							inneriter = 0;
							++outeriter;
							string data1;
							for (int i = 0; i < list2.size(); i++) {
								cout << list2[i] << endl;
								data1 = data1 + (list2[i]);
								data1 = data1 + "\n";
							}
							list1.push_back(data1);
							hr = pContainerToSearch->GetNextRow(hSearch);
						}
					}
					// Close the search handle to clean up
					pContainerToSearch->CloseSearchHandle(hSearch);
				}
				/*for (std::string s : list1)
				{
					cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< USer property >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> "<<endl;
					cout << s << endl;
				}*/
				//jobject result = env->NewObject(java_util_ArrayList, java_util_ArrayList_, list1.size());
				result = env->NewObject(java_util_ArrayList, java_util_ArrayList_, list1.size());
				for (std::string s : list1) {
					jstring element = env->NewStringUTF(s.c_str());
					env->CallBooleanMethod(result, java_util_ArrayList_add, element);
					env->DeleteLocalRef(element);
				}
				list1.clear();
				return result;

				if (SUCCEEDED(hr) && 0 == iCount)
					hr = S_FALSE;
				delete[] pszSearchFilter;
				delete[] szName;
				delete[] szDN;
				//return hr;
				//additional addon completed
				if (SUCCEEDED(hr))
				{
					if (S_FALSE == hr)
						wprintf(L"No user object could be found.\n");
					data = "No user object could be found.";
					    
				}
				else if (0x8007203e == hr)
				{
					wprintf(L"Could not execute query. An invalid filter was specified.\n");
					data = "Could not execute query. An invalid filter was specified.";
				}
				else
				{
					wprintf(L"Query failed to run. HRESULT: %x\n", hr);
					data = "Query failed to run.HRESULT: " + hr;
				}
			}
			else
			{
				wprintf(L"Could not execute query. Could not bind to the container.\n");
				data = "Could not execute query. Could not bind to the container.";
			}
			if (pContainerToSearch)
				pContainerToSearch->Release();
		}
		VariantClear(&var);
	}
	if (pObject)
		pObject->Release();

	delete[] pszBuffer;
	delete[] szPath;

	// Uninitialize COM
	CoUninitialize();
	result = env->NewObject(java_util_ArrayList, java_util_ArrayList_, list1.size());
	jstring element = env->NewStringUTF(data.c_str());
	env->CallBooleanMethod(result, java_util_ArrayList_add, element);
	env->DeleteLocalRef(element);
	list1.clear();
	return result;
}
int IS_BUFFER_ENOUGH(UINT maxAlloc, LPWSTR pszTarget, LPCWSTR pszSource, int toCopy)

{
	if (toCopy == -1)
	{
		toCopy = wcslen(pszSource);
	}
	return maxAlloc - (wcslen(pszTarget) + toCopy + 1);
}
int calculatedifference(SYSTEMTIME expdate)
{
	time_t rawtime = time(0);
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	/*std::cout << "-----------------------------Time date checking-------------------------------------------------" << "\n";
	std::cout<<"Year : " << timeinfo.tm_year+1900 <<"\n";
	std::cout << "month : " << timeinfo.tm_mon+1<< "\n";
	std::cout << "date : " << timeinfo.tm_mday  << "\n";
	std::cout << "hours : " << timeinfo.tm_hour << "\n";
	std::cout << "minute : " << timeinfo.tm_min  << "\n";
	std::cout << "Seconds : " << timeinfo.tm_sec << "\n";
	int y1, m1, dt1, hr1, mn1, ss1;
	int y2, m2, dt2, hr2, mn2, ss2;
	y1 = timeinfo.tm_year - 1900;
	m1 = timeinfo.tm_mon + 1;
	dt1 = timeinfo.tm_mday;
	hr1 = timeinfo.tm_hour;
	mn1 = timeinfo.tm_min;
	ss1 = timeinfo.tm_sec;
	cout << y1 << "  " << m1 << "  " << dt1 << "  " << hr1 << "   " << mn1<<"\n";
	struct tm t;
	memset(&t, 0, sizeof(tm)); // Initalize to all 0's
	t.tm_year = y1; // This is year-1900, so 112 = 2012
	t.tm_mon = m1;
	t.tm_mday = dt1;
	t.tm_hour = hr1;
	t.tm_min = mn1;
	t.tm_sec = ss1;
	time_t time_since_epoch1 = mktime(&t);
	std::cout << "------------------Expiry time-----------" << "\n";
	std::cout << "Year : " << expdate. wYear << "\n";
	std::cout << "month : " << expdate.wMonth << "\n";
	std::cout << "date : " << expdate.wDay << "\n";
	std::cout << "hours : " << expdate.wHour << "\n";
	std::cout << "minute : " << expdate.wMinute << "\n";
	std::cout << "Seconds : " << expdate.wSecond << "\n";
	std::cout << "#################################------------------------###################################################"<<"\n";
	y2 = expdate.wYear;
	m2 = expdate.wMonth;
	dt2 = expdate.wDay;
	hr2 = expdate.wHour;
	mn2 = expdate.wMinute;
	ss2 = expdate.wSecond;

	struct tm t1;
	memset(&t1, 0, sizeof(tm)); // Initalize to all 0's
	t1.tm_year = y2-1900; // This is year-1900, so 112 = 2012
	t1.tm_mon = m2;
	t1.tm_mday = dt2;
	t1.tm_hour = hr2;
	t1.tm_min = mn2;
	t1.tm_sec = ss2;
	time_t time_since_epoch2 = mktime(&t1);
	cout << "EPOCHTIME1 : " << rawtime<<"\n";
	cout << "EPOCHTIME2 : " << time_since_epoch2 << "\n";*/
	int yr = timeinfo.tm_year + 1900;
	int mn = timeinfo.tm_mon + 1;
	CTime t1(expdate.wYear, expdate.wMonth, expdate.wDay, expdate.wHour, expdate.wMinute, expdate.wSecond);
	CTime t2(yr, mn, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
	CTimeSpan ts = t1 - t2;
	int daydifference = ts.GetDays();
	int mindifference = ts.GetMinutes();
	int hrdifference = ts.GetHours();
	int di = ts.GetTotalHours();
	//std::cout << "#################################-------- difference check --------------###################################################" << "\n";
	//std::cout << "difference : " << ts.GetDays() << "\n";
	//std::cout << "days : " << daydifference << "   " << "minutes : " << mindifference << "  " << "hours : " << hrdifference << "\n";
	arr[0] = daydifference;
	arr[1] = mindifference;
	arr[2] = hrdifference;
	return 0;
}
char* convertDatetoString(VARIANT varDate2)
{
	char* lpszText2 = _com_util::ConvertBSTRToString(varDate2.bstrVal);
	//printf("The time value is : %s\n", lpszText2);
	return lpszText2;
}