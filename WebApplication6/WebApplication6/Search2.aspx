<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Search2.aspx.cs" Inherits="WebApplication6.Search2" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div style="height: 183px">
    
        <asp:TextBox ID="TextBox1" runat="server" Width="393px"></asp:TextBox>
&nbsp;&nbsp;&nbsp;
        <asp:Button ID="Button1" runat="server" onclick="Button1_Click" Text="Button" />
        <br />
        <asp:Repeater ID="Repeater1" runat="server">
        <ItemTemplate>
            <li>
                <%#Eval("ItemContent") %>
            </li>
        </ItemTemplate>
        </asp:Repeater>
    
    </div>
    </form>
</body>
</html>
