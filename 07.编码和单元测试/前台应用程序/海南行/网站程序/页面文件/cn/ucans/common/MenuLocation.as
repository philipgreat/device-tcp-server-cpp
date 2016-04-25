package cn.ucans.common
{
	import flash.display.DisplayObject;
	
	public class MenuLocation
	{
		public function MenuLocation()
		{
		}
		
		public function getDiaplayObject(name:String):DisplayObject{
			var display:DisplayObject = null;
			
			//韩勇 页面部分 
			
			if(name == "T_UserInfo_Role_Panel"){
				var tuirp:T_UserInfo_Role_Panel = new T_UserInfo_Role_Panel();
				display = tuirp as DisplayObject;
			}
			else if(name == "T_Operator_Role_Panel"){
				var C_T_Operator_Role_Panel:T_Operator_Role_Panel = new T_Operator_Role_Panel();
				display = C_T_Operator_Role_Panel as DisplayObject;
			}		
			else if(name == "Org_Manager_Panel"){
				var C_Org_Manager_Panel:Org_Manager_Panel = new Org_Manager_Panel();
				display = C_Org_Manager_Panel as DisplayObject;
			} 
			else if(name == "LinkMan_Manager_Panel"){
				var C_LinkMan_Manager_Panel:LinkMan_Manager_Panel = new LinkMan_Manager_Panel();
				display = C_LinkMan_Manager_Panel as DisplayObject;
			} 
			else if(name == "T_Terminal_Factory_Panel"){
				var C_T_Terminal_Factory_Panel:T_Terminal_Factory_Panel = new T_Terminal_Factory_Panel();
				display = C_T_Terminal_Factory_Panel as DisplayObject;
			} 
			else if(name == "UserInfo_LoginHis_00A_Panel"){
				var C_UserInfo_LoginHis_00A_Panel:UserInfo_LoginHis_00A_Panel = new UserInfo_LoginHis_00A_Panel();
				display = C_UserInfo_LoginHis_00A_Panel as DisplayObject;
			} 
			else if(name == "UserInfo_LoginHis_Other_Panel"){
				var C_UserInfo_LoginHis_Other_Panel:UserInfo_LoginHis_Other_Panel = new UserInfo_LoginHis_Other_Panel();
				display = C_UserInfo_LoginHis_Other_Panel as DisplayObject;
			}
			else if(name == "Terminal_Source_Manager_Panel"){
				var C_Terminal_Source_Manager_Panel:Terminal_Source_Manager_Panel = new Terminal_Source_Manager_Panel();
				display = C_Terminal_Source_Manager_Panel as DisplayObject;
			}
			else if(name == "Terminal_Source_Edit_00A_Panel"){
				var C_Terminal_Source_Edit_00A_Panel:Terminal_Source_Edit_00A_Panel = new Terminal_Source_Edit_00A_Panel();
				display = C_Terminal_Source_Edit_00A_Panel as DisplayObject;
			}
			else if(name == "Terminal_Source_Edit_Hotel_Panel"){
				var C_Terminal_Source_Edit_Hotel_Panel:Terminal_Source_Edit_Hotel_Panel = new Terminal_Source_Edit_Hotel_Panel();
				display = C_Terminal_Source_Edit_Hotel_Panel as DisplayObject;
			}
			else if(name == "Terminal_Source_ConfirmOpen_Hotel_Panel"){
				var C_Terminal_Source_ConfirmOpen_Hotel_Panel:Terminal_Source_ConfirmOpen_Hotel_Panel = new Terminal_Source_ConfirmOpen_Hotel_Panel();
				display = C_Terminal_Source_ConfirmOpen_Hotel_Panel as DisplayObject;
			}
			else if(name == "Org_Electronic_Documents_Panel"){
				var C_Org_Electronic_Documents_Panel:Org_Electronic_Documents_Panel = new Org_Electronic_Documents_Panel();
				display = C_Org_Electronic_Documents_Panel as DisplayObject;
			}
			else if(name == "Terminal_Settlement_Hotel_Panel"){
				var C_Terminal_Settlement_Hotel_Panel:Terminal_Settlement_Hotel_Panel = new Terminal_Settlement_Hotel_Panel();
				display = C_Terminal_Settlement_Hotel_Panel as DisplayObject;
			}
			else if(name == "Terminal_Settlement_Manager_Panel"){
				var C_Terminal_Settlement_Manager_Panel:Terminal_Settlement_Manager_Panel = new Terminal_Settlement_Manager_Panel();
				display = C_Terminal_Settlement_Manager_Panel as DisplayObject;
			}
			else if(name == "Terminal_Settlement_PutDown_Panel"){
				var C_Terminal_Settlement_PutDown_Panel:Terminal_Settlement_PutDown_Panel = new Terminal_Settlement_PutDown_Panel();
				display = C_Terminal_Settlement_PutDown_Panel as DisplayObject;
			}
			else if(name == "Terminal_Settlement_Query_Panel"){
				var C_Terminal_Settlement_Query_Panel:Terminal_Settlement_Query_Panel = new Terminal_Settlement_Query_Panel();
				display = C_Terminal_Settlement_Query_Panel as DisplayObject;
			}
			else if(name == "Terminal_Settlement_Repeated_Panel"){
				var C_Terminal_Settlement_Repeated_Panel:Terminal_Settlement_Repeated_Panel = new Terminal_Settlement_Repeated_Panel();
				display = C_Terminal_Settlement_Repeated_Panel as DisplayObject;
			}
			else if(name == "TR_Platform_Day_ReportTable"){
				var C_TR_Platform_Day_ReportTable:TR_Platform_Day_ReportTable = new TR_Platform_Day_ReportTable();
				display = C_TR_Platform_Day_ReportTable as DisplayObject;
			}
			else if(name == "TR_Platform_Month_ReportTable"){
				var C_TR_Platform_Month_ReportTable:TR_Platform_Month_ReportTable = new TR_Platform_Month_ReportTable();
				display = C_TR_Platform_Month_ReportTable as DisplayObject;
			}
			
			//郭建辉部分
			else if(name == "T_Terminal_Type_Panel"){
				var tttp:T_Terminal_Type_Panel = new T_Terminal_Type_Panel();
				display = tttp as DisplayObject;
			}else if(name == "T_Terminal_Service_Point_Panel"){
				var ttspp:T_Terminal_Service_Point_Panel = new T_Terminal_Service_Point_Panel();
				display = ttspp as DisplayObject;
			}else if(name == "T_Terminal_Parm_Panel"){
				var ttpp:T_Terminal_Parm_Panel = new T_Terminal_Parm_Panel();
				display = ttpp as DisplayObject;
			}else if(name == "T_Info_Pay_Type_Panel"){
				var tiptp:T_Info_Pay_Type_Panel = new T_Info_Pay_Type_Panel();
				display = tiptp as DisplayObject;
			}else if(name == "T_Public_Parm_Info_Panel"){
				var tppip:T_Public_Parm_Info_Panel = new T_Public_Parm_Info_Panel();
				display = tppip as DisplayObject;
			}else if(name == "T_Org_Reward_Panel"){
				var torp:T_Org_Reward_Panel = new T_Org_Reward_Panel();
				display = torp as DisplayObject;
			}else if(name == "T_Info_Type_Panel"){
				var titp:T_Info_Type_Panel = new T_Info_Type_Panel();
				display = titp as DisplayObject;
			}else if(name == "T_Info_Language_Panel"){
				var tilp:T_Info_Language_Panel = new T_Info_Language_Panel();
				display = tilp as DisplayObject;
			}else if(name == "Statement_TerminalAndRoomCount"){
				var starc:Statement_TerminalAndRoomCount = new Statement_TerminalAndRoomCount();
				display = starc as DisplayObject;
			}else if(name == "Statement_usability_Panel"){
				var sup:Statement_usability_Panel = new Statement_usability_Panel();
				display = sup as DisplayObject;
			}else if(name == "Statement_RentalHistoryForPlatform_Panel"){
				var srhfpp:Statement_RentalHistoryForPlatform_Panel = new Statement_RentalHistoryForPlatform_Panel();
				display = srhfpp as DisplayObject;
			}else if(name == "Statement_RentalHistoryHotel_Panel"){
				var srhhp:Statement_RentalHistoryHotel_Panel = new Statement_RentalHistoryHotel_Panel();
				display = srhhp as DisplayObject;
			}else if(name == "StatementOfMonth_Person_Panel"){
				var sompp:StatementOfMonth_Person_Panel = new StatementOfMonth_Person_Panel();
				display = sompp as DisplayObject;
			}else if(name == "StatementOfDay_Person_Panel"){
				var sodpp:StatementOfDay_Person_Panel = new StatementOfDay_Person_Panel();
				display = sodpp as DisplayObject;
			}else if(name == "StatementOfMonth_Agent_Panel"){
				var somap:StatementOfMonth_Agent_Panel = new StatementOfMonth_Agent_Panel();
				display = somap as DisplayObject;
			}else if(name == "StatementOfDay_Agent_Panel"){
				var sodap:StatementOfDay_Agent_Panel = new StatementOfDay_Agent_Panel();
				display = sodap as DisplayObject;
			}
			
			//宁海洋部分
			else if(name == "Black_Type_Manager"){
				var oBlack_Type_Manager:Black_Type_Manager = new Black_Type_Manager();
				display = oBlack_Type_Manager as DisplayObject;
			}
			else if(name == "White_Type_Manager"){
				var oWhite_Type_Manager:White_Type_Manager = new White_Type_Manager();
				display = oWhite_Type_Manager as DisplayObject;
			}
			else if(name == "Black_Type_His_Query"){
				var oBlack_Type_His_Query:Black_Type_His_Query = new Black_Type_His_Query();
				display = oBlack_Type_His_Query as DisplayObject;
			}
			else if(name == "White_Type_His_Query"){
				var oWhite_Type_His_Query:White_Type_His_Query = new White_Type_His_Query();
				display = oWhite_Type_His_Query as DisplayObject;
			}
			else if(name == "Black_Type_User_Report"){
				var oBlack_Type_User_Report:Black_Type_User_Report = new Black_Type_User_Report();
				display = oBlack_Type_User_Report as DisplayObject;
			} 
			else if(name == "SendObjectInfoListExport"){
				var oSendObjectInfoListExport:SendObjectInfoListExport = new SendObjectInfoListExport();
				display = oSendObjectInfoListExport as DisplayObject;
			} 
			else if(name == "SendObjectInfoList"){
				var oSendObjectInfoList:SendObjectInfoList = new SendObjectInfoList();
				display = oSendObjectInfoList as DisplayObject;
			} 
			else if(name == "CreateTaskInfo"){
				var oCreateTaskInfo:CreateTaskInfo = new CreateTaskInfo();
				display = oCreateTaskInfo as DisplayObject;
			} 
			else if(name == "ManagementTaskInfo"){
				var oManagementTaskInfo:ManagementTaskInfo = new ManagementTaskInfo();
				display = oManagementTaskInfo as DisplayObject;
			}
			else if(name == "ManagementTaskInfoAll"){
				var oManagementTaskInfoAll:ManagementTaskInfoAll = new ManagementTaskInfoAll();
				display = oManagementTaskInfoAll as DisplayObject;
			}
			else if(name == "QueryInfoSendTaskHis"){
				var oQueryInfoSendTaskHis:QueryInfoSendTaskHis = new QueryInfoSendTaskHis();
				display = oQueryInfoSendTaskHis as DisplayObject;
			}
			else if(name == "QueryInfoSendTaskHisAll"){
				var oQueryInfoSendTaskHisAll:QueryInfoSendTaskHisAll = new QueryInfoSendTaskHisAll();
				display = oQueryInfoSendTaskHisAll as DisplayObject;
			}
			else if(name == "ExportSendObjectHotelPhone"){
				var oExportSendObjectHotelPhone:ExportSendObjectHotelPhone = new ExportSendObjectHotelPhone();
				display = oExportSendObjectHotelPhone as DisplayObject;
			}
			else if(name == "VerificationPassesTask"){
				var oVerificationPassesTask:VerificationPassesTask = new VerificationPassesTask();
				display = oVerificationPassesTask as DisplayObject;
			}
			else if(name == "TR_Terminal_Hired_Fee_Report"){
				var oTR_Terminal_Hired_Fee_Report:TR_Terminal_Hired_Fee_Report = new TR_Terminal_Hired_Fee_Report();
				display = oTR_Terminal_Hired_Fee_Report as DisplayObject;
			}
			else if(name == "TR_Terminal_Use_Fee_Report"){
				var oTR_Terminal_Use_Fee_Report:TR_Terminal_Use_Fee_Report = new TR_Terminal_Use_Fee_Report();
				display = oTR_Terminal_Use_Fee_Report as DisplayObject;
			}
			else if(name == "TR_Terminal_In_Out_Report"){
				var oTR_Terminal_In_Out_Report:TR_Terminal_In_Out_Report = new TR_Terminal_In_Out_Report();
				display = oTR_Terminal_In_Out_Report as DisplayObject;
			}
			else if(name == "TR_Access_Log_HotelClick_Report"){
				var oTR_Access_Log_HotelClick_Report:TR_Access_Log_HotelClick_Report = new TR_Access_Log_HotelClick_Report();
				display = oTR_Access_Log_HotelClick_Report as DisplayObject;
			}
			else if(name == "TR_Access_Log_Click_Report"){
				var oTR_Access_Log_Click_Report:TR_Access_Log_Click_Report = new TR_Access_Log_Click_Report();
				display = oTR_Access_Log_Click_Report as DisplayObject;
			}
			else if(name == "TR_Access_Log_Click_TOP10_Report"){
				var oTR_Access_Log_Click_TOP10_Report:TR_Access_Log_Click_TOP10_Report = new TR_Access_Log_Click_TOP10_Report();
				display = oTR_Access_Log_Click_TOP10_Report as DisplayObject;
			}
			else if(name == "TR_Hotel_Date_Cash_Report"){
				var oTR_Hotel_Date_Cash_Report:TR_Hotel_Date_Cash_Report = new TR_Hotel_Date_Cash_Report();
				display = oTR_Hotel_Date_Cash_Report as DisplayObject;
			}
			else if(name == "TR_Hotel_Month_Cash_Report"){
				var oTR_Hotel_Month_Cash_Report:TR_Hotel_Month_Cash_Report = new TR_Hotel_Month_Cash_Report();
				display = oTR_Hotel_Month_Cash_Report as DisplayObject;
			}
			return display;
		}
	}
}