<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Search.aspx.cs" Inherits="WebApplication6.Search" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
    
        <asp:TextBox ID="tbxSearch" runat="server" Width="430px"></asp:TextBox>
&nbsp;
        <asp:Button ID="btnSearch" runat="server" onclick="btnSearch_Click" Text="搜搜" />
    
    </div>
    <asp:TextBox ID="tbxResult" runat="server" Height="311px" TextMode="MultiLine" 
        Width="429px"></asp:TextBox>
    </form>
</body>
</html>
