package cn.ucans.flex
{	
	[RemoteClass(alias="cn.ucans.flex.FlexRequest")]
	public class FlexRequest
	{	
		public var ope_id:int = -1;
		public var menu_id:String = "";
		public var type:int = -1;
		public var user_id:String = "";	
		public var org_id:String = "";	
		public var form:Object = new Object();

	}
}