package chobits.common;

public class PageUtil {
	
	public static int pageSize = 10;
	private int pageCount = 0;
	private int currentPageNO = 0;
	
	private int rowAllCount = 0;
	
	private int startRowNum = 0;
	private int endRowNum = 0;
	
	public static void init(int pageSize){
		PageUtil.pageSize = pageSize;
	}
	
	public PageUtil(int rowAllCount,int currentPageNO){
		
		this.rowAllCount = rowAllCount;
		
		if(this.rowAllCount <= 0){
			this.pageCount = 0;
			this.currentPageNO = 0;
			this.startRowNum = 0;
			this.endRowNum = 0;
		}else{			
			this.currentPageNO = currentPageNO;
			
			
			
			this.alaynerPageRows();
		}
		
	}
	
	private void alaynerPageRows(){
		
		if(rowAllCount > 0 && rowAllCount <= pageSize){
			
			this.pageCount = 1;
			this.currentPageNO = 1;
			this.startRowNum = 1;
			this.endRowNum = rowAllCount;
			
		}else{
			
			int tempPageCount = rowAllCount/pageSize;
			int tempPageYU = rowAllCount%pageSize;			
			
			if(tempPageYU == 0){
				if(currentPageNO > tempPageCount ){
					this.pageCount = tempPageCount;
					this.currentPageNO = tempPageCount;
					this.startRowNum = pageSize *(tempPageCount - 1) + 1;
					this.endRowNum = pageSize * tempPageCount;
				}else if(currentPageNO <= 0){
					this.pageCount = tempPageCount;
					this.currentPageNO = 1;
					this.startRowNum = 1;
					this.endRowNum = 10;
				}else{
					this.pageCount = tempPageCount;
					this.startRowNum = pageSize * (currentPageNO - 1) + 1;;
					this.endRowNum = pageSize * currentPageNO;
				}
			}else{
				
				tempPageCount++;
				
				if(currentPageNO >= tempPageCount){
					this.pageCount = tempPageCount;
					this.currentPageNO = tempPageCount;
					this.startRowNum = pageSize *(tempPageCount - 1) + 1;
					this.endRowNum = pageSize * (tempPageCount - 1) + tempPageYU;
				}else if(currentPageNO <= 0){
					this.pageCount = tempPageCount;
					this.currentPageNO = 1;
					this.startRowNum = 1;
					this.endRowNum = 10;
				}else{
					this.pageCount = tempPageCount;
					this.startRowNum = pageSize * (currentPageNO-1) + 1;;
					this.endRowNum = pageSize * currentPageNO ;
				}
			}
			
		}
		
	}

	public int getCurrentPageNO() {
		return currentPageNO;
	}

	public int getEndRowNum() {
		return endRowNum;
	}

	public int getPageCount() {
		return pageCount;
	}

	public int getStartRowNum() {
		return startRowNum;
	}

	public int getRowAllCount() {
		return rowAllCount;
	}

	public int getPageSize() {
		return pageSize;
	}

	public void setPageSize(int pageSize) {
		PageUtil.pageSize = pageSize;
	}
	
}
