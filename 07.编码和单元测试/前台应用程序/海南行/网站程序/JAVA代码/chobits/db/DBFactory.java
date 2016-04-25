package chobits.db;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import chobits.common.PageUtil;
import chobits.log.Logger;

public class DBFactory {
	
	public String SQL_Error_Message = null;
	public int SQL_Error_Code = 0;
	
	private DBBuilder builder = null;
	private Connection connection = null;
	private PreparedStatement statement = null;
	private ResultSet resultset = null;
	
	private Logger logger = new Logger(DBFactory.class.getName());
	private boolean isInterrupted = false;
	
	public DBFactory() {
		this.builder = new DBBuilder();
	}
	
	public void openTransaction(){
		this.connection = this.builder.getConnection();
		try {
			this.connection.setAutoCommit(false);
			logger.debug("------> db transaction start successfully <------");
		} catch (Exception e) {
			logger.error("Start Transaction Error :"+e.getMessage());
		}
	}
	
	public void interruptTransaction(){
		this.clearTempStatement();
		try {
			if(this.connection != null){
				this.isInterrupted = true;
				this.connection.rollback();
				logger.debug("------> db transaction interrupt,all operation rollback, interrupt successfully <------");
			}
		} catch (Exception e) {
			logger.debug("------> db transaction interrupt, error on rollbacking transaction <------");
			this.trackException(e);
		}
		this.close(connection, statement, resultset);
	}
	
	public long getOracleSequreNextValue(String seqName){
		this.clearTempStatement();
		String sql = "select "+seqName+".nextval as NEXTVALUE from dual";
		logger.debug(sql);
		long index = -1;
		try{
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
			    index = Long.parseLong(resultset.getObject("NEXTVALUE").toString());
			}			 
			resultset.close();
			statement.close();
		}catch(Exception e){
			this.trackException(e);
		}		
		return index;
	}
	
	public int excute(String sql){
		int excute_count = 0;
		this.clearTempStatement();
		boolean ok = false;
		try{
			logger.debug(sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);	
			excute_count = statement.executeUpdate();
			statement.close();
			ok = true;
		}catch(Exception e){
			this.trackException(e);
		}
		return excute_count;
	}
	
	public Map<String, Object> queryPage(String filelds,String table,String solution,int pageNo){
		this.clearTempStatement();
		Map<String, Object> return_map = null;
		PageUtil page = null;
		try{
			return_map = new HashMap<String, Object>();
			List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
			
			String count_sql = "";
			if(solution == null || solution.trim().length() == 0){
				count_sql = "select count(*) as TT_NO from "+table;
			}else{
				count_sql = "select count(*) as TT_NO from "+table + " where "+solution;
			}
			logger.debug(count_sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(count_sql);
			resultset = statement.executeQuery();
			int count = 0;
			while(resultset.next()){
				count = resultset.getInt("TT_NO");
			}
			
			resultset.close();
			statement.close();
			
			if(count > 0){
				page = new PageUtil(count, pageNo);
				String query_sql = "";
				if(solution == null || solution.trim().length() == 0){
					query_sql = "select * from (select "+filelds+" from "+table+") where rownum<="+page.getEndRowNum()+" and rownum>="+page.getStartRowNum();
				}else{
					query_sql = "select y.* from (select x.*,rownum numbers from (select "+filelds+" from "+table+" where "+solution+") x where rownum<="+page.getEndRowNum()+") y where y.numbers>="+page.getStartRowNum();
				}
				logger.debug(query_sql);
				if(connection == null){
					throw new Exception("db connection is null, please check db souce config currectly !");
				}
				statement = connection.prepareStatement(query_sql);
				resultset = statement.executeQuery();
				ResultSetMetaData rsmd = statement.getMetaData();
				int columnCount = rsmd.getColumnCount();

				while(resultset.next()){
					Map<String, Object> map = new HashMap<String, Object>();
					for(int i=1;i<=columnCount;i++){
						String type = rsmd.getColumnTypeName(i);
						String name = rsmd.getColumnName(i);
						if(type.equals("DATE")){
							String value = this.trackValue(resultset.getDate(name))+" "+this.trackValue(resultset.getTime(name));
							if(value.equals(" ")){
								value = "";
							}
							map.put(name, value);							
						}else{
							map.put(name, this.trackValue(resultset.getObject(name)));
						}
					}
					list.add(map);
				}				
				return_map.put(DBBuilder.ROW_COUNT, count);
				return_map.put(DBBuilder.PAGE_COUNT, page.getPageCount());
				return_map.put(DBBuilder.CURRENT_PAGE_NO, page.getCurrentPageNO());
			}else{
				return_map.put(DBBuilder.ROW_COUNT, 0);
				return_map.put(DBBuilder.PAGE_COUNT, 0);
				return_map.put(DBBuilder.CURRENT_PAGE_NO, 0);
			}
			return_map.put(DBBuilder.DATA_LIST, list);
			statement.close();
			resultset.close();
		}catch(Exception e){
			this.trackException(e);
		}
		return return_map;
	}
	
	public int getCount(String table, String solution){
		this.clearTempStatement();
		int index = -1;
		try{
			String sql = "";
			if(solution == null){
				sql = "select count(*) as TTT_NO from "+table;
			}else{
				sql = "select count(*) as TTT_NO from "+table+" where "+solution;
			}
			logger.debug(sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
			    index = resultset.getInt("TTT_NO");
			}			 
			statement.close();
			resultset.close();
		}catch(Exception e){
			this.trackException(e);
		}
		return index;
	}
	
	public int getCount(String table, String indexName,String solution){
		this.clearTempStatement();
		int index = -1;
		try{
			String sql = "";
			if(solution == null){
				sql = "select count("+indexName+") as TTT_NO from "+table;
			}else{
				sql = "select count("+indexName+") as TTT_NO from "+table+" where "+solution;
			}
			logger.debug(sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
			    index = resultset.getInt("TTT_NO");
			}			 
			statement.close();
			resultset.close();
		}catch(Exception e){
			this.trackException(e);
		}
		return index;
	}
	
	public int getDistinctCount(String table, String indexName,String solution){
		this.clearTempStatement();
		int index = -1;
		try{
			String sql = "";
			if(solution == null){
				sql = "select distinct count("+indexName+") as TTT_NO from "+table;
			}else{
				sql = "select distinct count("+indexName+") as TTT_NO from "+table+" where "+solution;
			}
			logger.debug(sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
			    index = resultset.getInt("TTT_NO");
			}			 
			resultset.close();
			statement.close();
		}catch(Exception e){
			this.trackException(e);
		}
		return index;
	}
	
	public List<Map<String, Object>> queryList(String sql){
		this.clearTempStatement();
		List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
		try{
			list = new ArrayList<Map<String, Object>>();
			logger.debug(sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			ResultSetMetaData rsmd = resultset.getMetaData();		
			int columnCount = rsmd.getColumnCount();
			while(resultset.next()){				
				Map<String, Object> map = new HashMap<String, Object>();				
				for(int i=1;i<=columnCount;i++){
					String columnName = rsmd.getColumnName(i);
					String columnType = rsmd.getColumnTypeName(i);	
					if(columnType.equals("DATE")){
						String value = this.trackValue(resultset.getDate(columnName)+" "+this.trackValue(resultset.getTime(columnName)));	
//						System.out.println("---------"+value);
						if(value.equals(" ")){
							value = "";
						}
						map.put(columnName, (String)value);
					}else{
						map.put(columnName, this.trackValue(resultset.getObject(columnName)));
					}		
				}
				list.add(map);		
			}
			statement.close();
			resultset.close();
		}catch(Exception e){
			this.trackException(e);
		}
		return list;	
	}
	
	public List<String> queryListForSingleField(String column,String table,String soulution){
		this.clearTempStatement();
		List<String> list = new ArrayList<String>();
		try{
			list = new ArrayList<String>();
			String sql = "select "+column+ " from "+table +" where "+soulution;
			logger.debug(sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			ResultSetMetaData rsmd = resultset.getMetaData();		
			int columnCount = rsmd.getColumnCount();
			while(resultset.next()){
				String value = "";
				for(int i=1;i<=columnCount;i++){
					String columnName = rsmd.getColumnName(i);
					String columnType = rsmd.getColumnTypeName(i);	
					
					if(columnType.equals("DATE")){
						value = this.trackValue(resultset.getDate(columnName))+" "+this.trackValue(resultset.getTime(columnName));
						if(value.equals(" ")){
							value = "";
						}
					}else{
						value = this.trackValue(resultset.getObject(columnName));
					}		
				}
				list.add(value);		
			}
			statement.close();
			resultset.close();
		}catch(Exception e){
			this.trackException(e);
		}
		return list;	
	}
	
	public Map<String, Object> queryMap(String sql){
		this.clearTempStatement();
		Map<String, Object> map = null;
		try{	
			map = new HashMap<String, Object>();
			logger.debug(sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			ResultSetMetaData rsmd = resultset.getMetaData();		
			int columnCount = rsmd.getColumnCount();
			while(resultset.next()){	
				
				for(int i=1;i<=columnCount;i++){
					String columnName = rsmd.getColumnName(i);
					String columnType = rsmd.getColumnTypeName(i);	
					if(columnType.equals("DATE")){
						String value = this.trackValue(resultset.getDate(columnName)+" "+this.trackValue(resultset.getTime(columnName)));	
						if(value.equals(" ")){
							value = "";
						}
						map.put(columnName, (String)value);
					}else{
						map.put(columnName, this.trackValue(resultset.getObject(columnName)));
					}		
				}
				break;	
			}
			statement.close();
			resultset.close();
		}catch(Exception e){
			this.trackException(e);
		}
		if(map.size()>0){
			return map;
		}else{
			return null;
		}
	}
	
	private void clearTempStatement(){
		this.SQL_Error_Code = 0;
		this.SQL_Error_Message = null;
	}
	
	public void closeTransaction(){
		try {
			if(this.connection != null && isInterrupted == false){
				this.connection.commit();
				logger.debug("------> db transaction commit successfully <------");
			}
			
		} catch (Exception e) {
			logger.debug("------> db transaction commit error !!! <------");
			this.trackException(e);
		}
		this.close(connection, statement, resultset);
	}
	
	public void close(Connection connection, PreparedStatement statement, ResultSet resultset){
		try{
			if(resultset != null){
				resultset.close();
			}			
			if(statement != null){
				statement.close();
			}			
			if(connection != null){
				connection.close();
			}
			logger.debug("db release souce complete !");
		}catch(Exception e){
			if(resultset != null){
				resultset = null;
			}			
			if(statement != null){
				statement = null;
			}			
			if(connection != null){
				connection = null;
			}
			logger.error("database statement close failed,wait gc lose souce later \n"+e.getMessage());
		}
	}
	
	public String trackValue(Object object){
		if(object == null || object.toString().trim().length()==0 || object.toString().trim().equals("null") || object.toString().trim().equals("NULL")){
			return "";
		}else{
			return object.toString();
		}
	}
	
	public void trackException(Exception e){
		try {
			this.connection.rollback();
		} catch (SQLException e1) {
			logger.error("transaction make error ,roolback failed "+e1.getMessage());
		}
		if(e instanceof SQLException){
			SQLException sql_exception = (SQLException) e; 
			this.SQL_Error_Code = sql_exception.getErrorCode();
			this.SQL_Error_Message = sql_exception.getMessage();
		}else{
			this.SQL_Error_Code = -1000000;
			this.SQL_Error_Message = e.getMessage();
		}
		logger.error(this.SQL_Error_Code+"<->"+this.SQL_Error_Message);
		this.close(connection, statement, resultset);
	}
	
	public int queryIntClumn(String sql){
		this.clearTempStatement();
		int returnN=0;
		try{
			logger.debug(sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			if(resultset.next()){
				returnN=resultset.getInt(1);
			}
			
			statement.close();
			resultset.close();
		}catch(Exception e){
			this.trackException(e);
		}
		return returnN;	
	}
	
	public String nextMaxValue(String table,String column){
		return "(select max("+column+")+1 from "+table+")";
	}
	
	public static void main(String[] args){
		DBBuilder.initCommonType();
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		factory.excute("insert into T_Terminal_Change_His(Imei_Number,Uim_Number,Msisdn,Change_Type,User_ID,Change_Time) values ('666666666666666','666666666666666','66666666666','01','admin',sysdate)");
		System.out.println(factory.SQL_Error_Code+","+factory.SQL_Error_Message);
		factory.closeTransaction();
	}

}
