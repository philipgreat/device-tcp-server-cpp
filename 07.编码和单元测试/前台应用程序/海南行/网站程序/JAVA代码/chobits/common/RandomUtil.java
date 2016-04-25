package chobits.common;

import java.util.Random;

public class RandomUtil {
	
	private int length = 0;  
	private long basic = 0;	
	private char[] seed = {
								'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'q',
								'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's',
								'd','f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 
								'b', 'n', 'm','Q','W', 'E', 'R', 'T', 'Y', 'U', 'I', 
								'O', 'P', 'A', 'S', 'D','F', 'G', 'H', 'J', 'K', 'L', 
								'Z', 'X', 'C', 'V', 'B', 'N', 'M'
							};	
	
	public RandomUtil(int size) throws Exception {  
		
		if(size<=0){			
			
			throw new Exception("The size of random data requested is not less than 1");
					
		}else{
			length = size;
			
			for(int i=0;i<size-1;i++){
				basic *= 10;
			}
		}
		
	}
	
	public void setRandomSeed(char[] inputseed){
		
		seed = inputseed;
		
	}
	
	public String getRegionRandomString(){ 
		        
         Random random = new Random(); 
         StringBuffer string = new StringBuffer();
         
         for (int i = 0; i < length; i++){
        	 
        	 string.append(seed[Math.abs(random.nextInt()) % seed.length]);
        	 
         }
         try {
			Thread.sleep(10);
		} catch (InterruptedException e) {
			// TODO 自动生成 catch 块
			e.printStackTrace();
		}
         return string.toString();
		
	}
	
	public static void main(String[] args) throws Exception{
		
		char[] seed = {
				'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
				'Q','W', 'E', 'R', 'T', 'Y', 'U', 'I', 
				'O', 'P', 'A', 'S', 'D','F', 'G', 'H', 'J', 'K', 'L', 
				'Z', 'X', 'C', 'V', 'B', 'N', 'M'
			};
		
		RandomUtil ru = new RandomUtil(5);
		ru.setRandomSeed(seed);
		
		System.out.println(ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString()+"-"+ru.getRegionRandomString());
		
	}
	
}

