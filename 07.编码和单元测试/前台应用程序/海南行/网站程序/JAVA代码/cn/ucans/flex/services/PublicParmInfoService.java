package cn.ucans.flex.services;

import java.util.List;
import java.util.Map;


import chobits.db.DBBuilder;
import chobits.log.Logger;
import cn.ucans.bean.T_PUBLIC_PARM_INFO;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;

public class PublicParmInfoService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------�����������ù������� ����-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9050:
					reponse = this.initPublicParmInfo(request);
					break;
				case 9051:
					reponse = this.addPublicParmInfo(request);
					break;
				case 9052:
					reponse = this.updatePublicParmInfo(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------�����������ù������� �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------�����������ù������� ִ��ʧ��-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initPublicParmInfo(FlexRequest request) throws Exception{
		
		String select_publicParmInfoSQL="Select Notes,Parm_Key,Parm_Values From T_Public_Parm_Info";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_publicParmInfoSQL);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�����������ã���ȡ��������������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		return reponse;
	}
	
	
	
	private FlexReponse addPublicParmInfo(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_PUBLIC_PARM_INFO publicParmInfo = (T_PUBLIC_PARM_INFO) request.getBean("publicParmInfo");
		int count = builder.getCount("T_Public_Parm_Info", "Parm_Key='"+publicParmInfo.PARM_KEY+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ӹ����������ã���ȡ��������������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count>0){
				throw new Exception("��ӹ����������ã�������ʶΪ��"+publicParmInfo.PARM_KEY+"����������Ϣ�Ѿ����ڣ�");
			}
		}
		
		String sql = "Insert Into T_Public_Parm_Info(Parm_Key,Parm_Values,Update_User_ID,Update_Time,Notes) values('"
			+publicParmInfo.PARM_KEY+ "','"+publicParmInfo.PARM_VALUES+"','" +publicParmInfo.UPDATE_USER_ID+ "',sysdate,'"+publicParmInfo.NOTES+"')";
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ӹ����������ó���ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"��ӹ�����������,������ʶΪ��"+publicParmInfo.PARM_KEY+"������ӳɹ���");
		return reponse;
	}
	
	private FlexReponse updatePublicParmInfo(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_PUBLIC_PARM_INFO publicParmInfo = (T_PUBLIC_PARM_INFO) request.getBean("publicParmInfoForUpdate");
		T_PUBLIC_PARM_INFO oldPublicParmInfoForUpdate = (T_PUBLIC_PARM_INFO) request.getBean("oldPublicParmInfoForUpdate");
		int count = builder.getCount("T_Public_Parm_Info", "Parm_Key='"+oldPublicParmInfoForUpdate.PARM_KEY+"' and Parm_Values='"+oldPublicParmInfoForUpdate.PARM_VALUES+"' and Notes='"+oldPublicParmInfoForUpdate.NOTES+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�޸Ĺ����������ã���ȡ��������������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count<=0){
				throw new Exception("�޸Ĺ����������ã�����ѡ�Ĺ�������������Ϣ�Ѿ������ڻ��Ѿ����޸ģ���ȷ�Ϻ����޸ģ�");
			}
		}
		
		String sql = "Update T_Public_Parm_Info set Parm_Values='"+publicParmInfo.PARM_VALUES
		            +"',Update_User_ID='"+publicParmInfo.UPDATE_USER_ID
		            + "',Update_Time=sysdate "
		            + ", Notes='"+publicParmInfo.NOTES
		            + "' where Parm_Key='"+publicParmInfo.PARM_KEY+"'";
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�޸Ĺ����������ó���ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"�޸Ĺ�������������ӳɹ���");
		return reponse;
	}
}
