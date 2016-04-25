package cn.ucans.flex.services;

import java.util.Map;

import chobits.db.DBFactory;
import chobits.log.Logger;
import cn.ucans.flex.FlexRequest;

public class FlexService {
	
	private Logger logger = new Logger(FlexService.class.getName());
	
	public void checkRole(FlexRequest request) throws Exception{
		logger.debug("--------->开始验证用户权限<-----------");
		try{			
			String user_id = request.user_id;
			String menu_id = request.menu_id;
			String org_id = request.org_id;
			String menu_name = "";
			
			logger.debug("用户编号："+user_id+",菜单编号："+menu_id+",组织编号："+org_id);
			
			if(!menu_id.equals("ONJM8-1W1UN-A544J-PSNK7-PXV95-2D8RT-T3PX8-AJH2Q-L5RNW-PRM7E-W93LA-HZN8D")){
				DBFactory factory = new DBFactory();
				factory.openTransaction();
				
				Map<String,Object> map_org = factory.queryMap("select Status from T_Org_Info where Org_ID='"+org_id+"'");
				if(factory.SQL_Error_Code != 0){
					throw new Exception("校验用户（"+user_id+"，"+org_id+"）权限，获取组织信息"+org_id+"出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(map_org == null){
						factory.interruptTransaction();
						throw new Exception("您当前所管辖的组织（"+org_id+"）已经不存在，无法执行本操作！");
					}
				}
				
				String org_status = (String) map_org.get("STATUS");
				if(org_status.equals("1")){
					factory.interruptTransaction();
					throw new Exception("您当前所管辖的组织（"+org_id+"）已经被注销，无法执行本操作！");
				}
				
				Map<String,Object> map_role_id = factory.queryMap("select Role_ID from T_User_Info where User_ID='"+user_id+"'");
				if(factory.SQL_Error_Code != 0){
					throw new Exception("校验用户（"+user_id+"，"+org_id+"）权限，获取用户"+user_id+"的角色信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(map_role_id == null){
						factory.interruptTransaction();
						throw new Exception("您的帐户当前没有被赋予任何系统角色，无法执行本操作！");
					}
				}
				
				String role_id = (String) map_role_id.get("ROLE_ID");
				int count_menu = factory.getCount("T_Role_Menu", "Role_ID='"+role_id+"'");
				if(factory.SQL_Error_Code != 0){
					throw new Exception("校验用户（"+user_id+"，"+org_id+"）权限，获取用户"+user_id+"的角色信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(count_menu == 0){
						factory.interruptTransaction();
						throw new Exception("您的帐户所属角色没有加载任何功能菜单，无法执行本操作！");
					}
				}
				
				Map<String,Object> map_data = factory.queryMap("select Menu_Name from T_Role_Menu where Role_ID='"+role_id+"' and Program_Name='"+menu_id+"'");
				menu_name = (String) map_data.get("MENU_NAME");
				if(factory.SQL_Error_Code != 0){
					throw new Exception("校验用户（"+user_id+"，"+org_id+"）权限，获取用户"+user_id+"的角色功能菜单"+menu_id+"出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(map_data == null){
						factory.interruptTransaction();
						throw new Exception("您的角色功能权限不包含"+menu_name+"，无法执行本操作！");
					}
				}
				
				//String menu_name = (String) map_data.get("MENU_NAME");
				factory.closeTransaction();
				if(factory.SQL_Error_Code != 0){
					throw new Exception("校验用户权限，提交验证权限事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}
			}
			logger.debug("功能菜单权限验证通过：可以进行"+menu_name+"操作！");
		}catch(Exception e){
			logger.debug("--------->验证用户权限失败："+e.getMessage()+"<-----------");
			throw new Exception("验证用户权限失败："+e.getMessage());
		}
		logger.debug("--------->验证用户权限成功<-----------");
	}
}
