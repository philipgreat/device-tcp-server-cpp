package cn.ucans.role;

public class OrgRoleEnigne {
//	Org_ID						Varchar2(10),	--��֯ID(ƽ̨ʹ��HAPPY)
//	Org_Name					Varchar2(64),	--��֯����
//	Org_Type					Varchar2(1),	--��֯����(0ƽ̨,1�ն˳���,2������,3�Ƶ�,4���ҵ��)
//	Superior_Org_ID				Varchar2(10),	--�ϼ���֯ID(�Ƶꡢ���ҵ������һ���ض��Ĵ����̣�������֯ʱ�������ֵ)
//	Hotel_Grade					Number(1),		--�Ƶ��Ǽ�(3-���Ǽ���4-���Ǽ���5-���Ǽ���9-��������Ƶ�)
//	Address						Varchar2(64),	--��֯��ַ
//	Zip_Code					Varchar2(6),	--��������
//	Fax_No						Varchar2(16),	--�������
//	Contract_No					Varchar2(32),	--��ͬ���
//	Contract_Date				Varchar2(8),	--ǩԼ����(yyyymmdd)
//	Room_Count					Number(5),		--�Ƶ�ķ���������ֻ�ԾƵ������壩
//	Send_SM_Max_Daily			Number(8),		--�Ƶꡢ�����ÿ�췢�͹������������
//	Status						Varchar2(1),	--��֯״̬(0����,1ע��)
//	Create_User_ID				Varchar2(16),	--������֯�����Ĳ���ԱID
//	Create_Time					Date,			--������֯����ʱ��
//	Last_Update_Time			Date,			--����޸�ʱ��
//	Notes						Varchar2(64)	--��ע
	
	private String xml = null;
	
	public OrgRoleEnigne(String xml){
		this.xml = xml;
	}
	
	public String toXML(){
		return null;		
	}
}
