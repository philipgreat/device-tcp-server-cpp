package cn.ucans.role;

public class OrgRoleEnigne {
//	Org_ID						Varchar2(10),	--组织ID(平台使用HAPPY)
//	Org_Name					Varchar2(64),	--组织名称
//	Org_Type					Varchar2(1),	--组织类型(0平台,1终端厂家,2代销商,3酒店,4广告业主)
//	Superior_Org_ID				Varchar2(10),	--上级组织ID(酒店、广告业主属于一个特定的代销商，其它组织时，则填空值)
//	Hotel_Grade					Number(1),		--酒店星级(3-三星级、4-四星级、5-五星级、9-连锁商务酒店)
//	Address						Varchar2(64),	--组织地址
//	Zip_Code					Varchar2(6),	--邮政编码
//	Fax_No						Varchar2(16),	--传真号码
//	Contract_No					Varchar2(32),	--合同编号
//	Contract_Date				Varchar2(8),	--签约日期(yyyymmdd)
//	Room_Count					Number(5),		--酒店的房间数量（只对酒店有意义）
//	Send_SM_Max_Daily			Number(8),		--酒店、广告商每天发送广告短信最大条数
//	Status						Varchar2(1),	--组织状态(0正常,1注销)
//	Create_User_ID				Varchar2(16),	--创建组织机构的操作员ID
//	Create_Time					Date,			--创建组织机构时间
//	Last_Update_Time			Date,			--最近修改时间
//	Notes						Varchar2(64)	--备注
	
	private String xml = null;
	
	public OrgRoleEnigne(String xml){
		this.xml = xml;
	}
	
	public String toXML(){
		return null;		
	}
}
