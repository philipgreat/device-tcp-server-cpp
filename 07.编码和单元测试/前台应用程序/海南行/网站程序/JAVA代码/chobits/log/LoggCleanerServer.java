package chobits.log;

import java.io.File;

import chobits.common.DateUtil;

public class LoggCleanerServer implements Runnable{
	
	private DateUtil du = null;
	private String log_forder_path = null;
	private String temp_forder_path = null;
	
	private Logger logger = new Logger(LoggCleanerServer.class.getName());
	
	public LoggCleanerServer(String log_folderpath, String temp_forder_path){
		du = new DateUtil();
		this.log_forder_path = log_folderpath;
		this.temp_forder_path = temp_forder_path;
	}
	
	public void run() {
		while(true){			
			try{
				if(this.isTimeOK()){
					this.deleteLogFile();
					this.deleteTempFile();
				}	
				Thread.sleep(60*60*1000L);
			}catch (Exception e){
				e.printStackTrace();
				logger.info("start log cleaner task failed: \n"+e.getMessage());
			}
		}		
	}
	
	private boolean isTimeOK(){
		String time_now = this.du.getYMDHMSTime(DateUtil.Format_EN_HH_mm_ss);
		if(time_now.compareTo("21:00:00") > 0 && time_now.compareTo("23:59:00") < 0){
			return true;
		}else{
			return false;
		}
	}
	
	private void deleteLogFile(){
		logger.info("---------------Start Clear Log Task--------------");
		File file = new File(this.log_forder_path);
		if(file.exists()){
			logger.info("get forder path file "+this.log_forder_path);
			File[] elems_file = file.listFiles();
			logger.info("count of file checked later is  "+elems_file.length);
			if(elems_file.length > 0){
				int count = 0;
				for(int i=0;i<elems_file.length;i++){
					String fileName = elems_file[i].getName();
					String[] elems = fileName.split("_");
					long time_equal = this.du.getMillisecondTime(elems[1], DateUtil.Format_EN_yyyyMMddHHmmss);
					long nowlongtime = this.du.getMillisecondTime();
					nowlongtime = nowlongtime - 30*24*60*60*1000L;
					if(time_equal < nowlongtime){
						logger.info("found expired file for deleting: "+elems_file[i].getName());
						elems_file[i].delete();
						count++;
					}					
				}
				logger.info("this task's count of deleting file is "+count);
			}
		}else{
			logger.info("found forder not found path file "+this.log_forder_path);
		}
		logger.info("---------------End Clear Log Task--------------");		
	}
	
	private void deleteTempFile(){
		logger.info("---------------Start Clear TempFile Task--------------");
		File file = new File(this.temp_forder_path);
		if(file.exists()){
			logger.info("get forder path file "+this.temp_forder_path);
			File[] elems_file = file.listFiles();
			logger.info("count of file checked later is  "+elems_file.length);
			if(elems_file.length > 0){
				int count = 0;
				for(int i=0;i<elems_file.length;i++){
					elems_file[i].delete();				
				}
				logger.info("this task's count of deleting file is "+count);
			}
		}else{
			logger.info("found forder not found path file "+this.temp_forder_path);
		}
		logger.info("---------------End Clear TempFile Task--------------");		
	}
}
