package com.example.dibudaoying;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Map;
import java.util.Set;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

public class HuaweiIOT {
    static String HUAWEINAME="hw050513431";           //华为账号名
    static String IAMINAME="IAMuser";             //IAM账户名
    static String IAMPASSWORD="yy20030213";          //IAM账户密码
    static String project_id="f4a07a4cfdc84f028ecbecb7b2fd3133";           //产品ID
    static String device_id="64a98f799415fc7a573be8f0_Temp_Humi_Gas_Detector";            //设备ID
    String service_id="monitor";           //服务ID
    String commands="temp_monitor";             //命令名称
    static String token="";

    public static String getdev(String value) throws Exception
    {
        String output = null;
        String strurl="https://db0777e9b1.st1.iotda-app.cn-north-4.myhuaweicloud.com:443"+"/v5/iot/%s/devices/%s/shadow";
        strurl = String.format(strurl,project_id,device_id);
        String  token=gettoken();
        URL url = new URL(strurl);
        HttpURLConnection urlCon = (HttpURLConnection)url.openConnection();
        urlCon.addRequestProperty("Content-Type", "application/json");
        urlCon.addRequestProperty("X-Auth-Token",token);
        urlCon.connect();
        InputStreamReader is = new InputStreamReader(urlCon.getInputStream());
        BufferedReader bufferedReader = new BufferedReader(is);
        StringBuffer strBuffer = new StringBuffer();
        String line = null;
        while ((line = bufferedReader.readLine()) != null) {
            strBuffer.append(line);
        }
        is.close();
        urlCon.disconnect();
        String result = strBuffer.toString();
        System.out.println(result);
        ObjectMapper mapper = new ObjectMapper();
        try {
            JsonNode jsonNode = mapper.readTree(result);
            JsonNode propertiesNode = jsonNode.path("shadow").get(0).path("reported").path("properties");

            // 提取属性值
            String tempValue = propertiesNode.path("Temp_ControlModule").asText();
            String humidValue = propertiesNode.path("Humid_ControlModule").asText();
            String gasValue = propertiesNode.path("Gas_ControlModule").asText();
            String tdsValue = propertiesNode.path("TDS_monitor").asText();

            // 打印结果
            System.out.println("Temp_ControlModule: " + tempValue);
            System.out.println("Humid_ControlModule: " + humidValue);
            System.out.println("Gas_ControlModule: " + gasValue);
            System.out.println("TDS_monitor: " + tdsValue);
            if(value=="tempvalue")
            {output=tempValue;} else if (value=="humidvalue") {
                output=humidValue;
            } else if (value=="gasvalue") {
                output=gasValue;
            } else if (value=="tdsvalue") {
                output=tdsValue;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return output;
    }
    public static String gettoken()throws Exception
    {
        String strurl="";
        strurl="https://iam.cn-north-4.myhuaweicloud.com"+"/v3/auth/tokens?nocatalog=false";
        String tokenstr="{"+"\""+"auth"+"\""+": {"+"\""+"identity"+"\""+": {"+"\""+"methods"+"\""+": ["+"\""+"password"+"\""+"],"+"\""+"password"+"\""+": {"+"\""+"user"+"\""+":{"+"\""+"domain\": {\"name\": \""+HUAWEINAME+"\"},\"name\": \""+IAMINAME+"\",\"password\": \""+IAMPASSWORD+"\"}}},\"scope\": {\"project\": {\"name\": \"cn-north-4\"}}}}";
        URL url = new URL(strurl);
        HttpURLConnection urlCon = (HttpURLConnection)url.openConnection();
        urlCon.addRequestProperty("Content-Type", "application/json;charset=utf8");

        urlCon.setDoOutput(true);
        urlCon.setRequestMethod("POST");
        urlCon.setUseCaches(false);
        urlCon.setInstanceFollowRedirects(true);
        urlCon.connect();


        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(urlCon.getOutputStream(),"UTF-8"));
        writer.write(tokenstr);
        writer.flush();
        writer.close();
        Map headers = urlCon.getHeaderFields();
        Set<String> keys = headers.keySet();
        /*for( String key : keys ){
            String val = urlCon.getHeaderField(key);
            System.out.println(key+"    "+val);
        }*/
        String token = urlCon.getHeaderField("X-Subject-Token");
        System.out.println("X-Subject-Token"+"："+token);
        return  token;
    }
}
