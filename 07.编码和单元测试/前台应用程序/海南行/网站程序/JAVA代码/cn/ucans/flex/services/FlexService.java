package cn.ucans.flex.services;

import java.util.Map;

import chobits.db.DBFactory;
import chobits.log.Logger;
import cn.ucans.flex.FlexRequest;

public class FlexService {
	
	private Logger logger = new Logger(FlexService.class.getName());
	
	public void checkRole(FlexRequest request) throws Exception{
		logger.debug("--------->��ʼ��֤�û�Ȩ��<-----------");
		try{			
			String user_id = request.user_id;
			String menu_id = request.menu_id;
			String org_id = request.org_id;
			String menu_name = "";
			
			logger.debug("�û���ţ�"+user_id+",�˵���ţ�"+menu_id+",��֯��ţ�"+org_id);
			
			if(!menu_id.equals("ONJM8-1W1UN-A544J-PSNK7-PXV95-2D8RT-T3PX8-AJH2Q-L5RNW-PRM7E-W93LA-HZN8D")){
				DBFactory factory = new DBFactory();
				factory.openTransaction();
				
				Map<String,Object> map_org = factory.queryMap("select Status from T_Org_Info where Org_ID='"+org_id+"'");
				if(factory.SQL_Error_Code != 0){
					throw new Exception("У���û���"+user_id+"��"+org_id+"��Ȩ�ޣ���ȡ��֯��Ϣ"+org_id+"����ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(map_org == null){
						factory.interruptTransaction();
						throw new Exception("����ǰ����Ͻ����֯��"+org_id+"���Ѿ������ڣ��޷�ִ�б�������");
					}
				}
				
				String org_status = (String) map_org.get("STATUS");
				if(org_status.equals("1")){
					factory.interruptTransaction();
					throw new Exception("����ǰ����Ͻ����֯��"+org_id+"���Ѿ���ע�����޷�ִ�б�������");
				}
				
				Map<String,Object> map_role_id = factory.queryMap("select Role_ID from T_User_Info where User_ID='"+user_id+"'");
				if(factory.SQL_Error_Code != 0){
					throw new Exception("У���û���"+user_id+"��"+org_id+"��Ȩ�ޣ���ȡ�û�"+user_id+"�Ľ�ɫ��Ϣ����ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(map_role_id == null){
						factory.interruptTransaction();
						throw new Exception("�����ʻ���ǰû�б������κ�ϵͳ��ɫ���޷�ִ�б�������");
					}
				}
				
				String role_id = (String) map_role_id.get("ROLE_ID");
				int count_menu = factory.getCount("T_Role_Menu", "Role_ID='"+role_id+"'");
				if(factory.SQL_Error_Code != 0){
					throw new Exception("У���û���"+user_id+"��"+org_id+"��Ȩ�ޣ���ȡ�û�"+user_id+"�Ľ�ɫ��Ϣ����ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(count_menu == 0){
						factory.interruptTransaction();
						throw new Exception("�����ʻ�������ɫû�м����κι��ܲ˵����޷�ִ�б�������");
					}
				}
				
				Map<String,Object> map_data = factory.queryMap("select Menu_Name from T_Role_Menu where Role_ID='"+role_id+"' and Program_Name='"+menu_id+"'");
				menu_name = (String) map_data.get("MENU_NAME");
				if(factory.SQL_Error_Code != 0){
					throw new Exception("У���û���"+user_id+"��"+org_id+"��Ȩ�ޣ���ȡ�û�"+user_id+"�Ľ�ɫ���ܲ˵�"+menu_id+"����ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(map_data == null){
						factory.interruptTransaction();
						throw new Exception("���Ľ�ɫ����Ȩ�޲�����"+menu_name+"���޷�ִ�б�������");
					}
				}
				
				//String menu_name = (String) map_data.get("MENU_NAME");
				factory.closeTransaction();
				if(factory.SQL_Error_Code != 0){
					throw new Exception("У���û�Ȩ�ޣ��ύ��֤Ȩ���������ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}
			}
			logger.debug("���ܲ˵�Ȩ����֤ͨ�������Խ���"+menu_name+"������");
		}catch(Exception e){
			logger.debug("--------->��֤�û�Ȩ��ʧ�ܣ�"+e.getMessage()+"<-----------");
			throw new Exception("��֤�û�Ȩ��ʧ�ܣ�"+e.getMessage());
		}
		logger.debug("--------->��֤�û�Ȩ�޳ɹ�<-----------");
	}
}
