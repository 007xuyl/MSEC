
/**
 * Tencent is pleased to support the open source community by making MSEC available.
 *
 * Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the GNU General Public License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. You may 
 * obtain a copy of the License at
 *
 *     https://opensource.org/licenses/GPL-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the 
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions
 * and limitations under the License.
 */


//�����������̳߳���ִ�е�����
//��ں�����run()
package ngse.remote_shell;



import org.codehaus.jackson.map.ObjectMapper;
import org.json.JSONObject;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.Socket;

/**
 * Created by Administrator on 2016/2/4.
 */
public class Worker implements Runnable {
    private  Socket socket;//����ǰ�����ӵ�socket

    public Worker(Socket s)
    {
        socket = s;
    }

    private void returnErrorMessage(String msg)
    {
        JsonRPCResponseBase r = new JsonRPCResponseBase();
        ObjectMapper objectMapper = new ObjectMapper();
        r.setMessage(msg);
        r.setStatus(100);
        try {
            String s = objectMapper.writeValueAsString(r);
            socket.getOutputStream().write(s.getBytes());
        }
        catch (Exception e)
        {
            e.printStackTrace();
            return ;
        }

    }

    //���÷�����ƣ�
    // 1.��������json�ַ����е�handleClass�ֶΣ�ʵ����ʵ�ʵĴ�����
    // 2. �Զ�������json�ַ���ʵ����Ϊjava�࣬�൱�ڲ������
    // 3. �Զ�������󷵻ص�java��ʵ�������л�Ϊjson �ַ�������ΪӦ���͸�ǰ��
    private String handleRequest(String jsonStr, InputStream in, OutputStream out)
    {
        JSONObject jsonObject = new JSONObject(jsonStr);
        String handleClassStr = jsonObject.getString("handleClass");
        JSONObject requestBodyObj = jsonObject.getJSONObject("requestBody");
        ObjectMapper objectMapper = new ObjectMapper();
        try
        {
            Class<?> clazz = Class.forName(handleClassStr);
            //ʵ����handle classָ������
            JsonRPCHandler handler = (JsonRPCHandler) clazz.newInstance();

            //��ø�������з������ҵ�exec����������
            Method[] methods = clazz.getMethods();
            boolean execFound = false;
            for (int i = 0; i <methods.length ; i++) {

                if (methods[i].getName().equals("exec")) {
                    execFound = true;

                    //exec�����Ĳ��������ͣ���json�����л�Ϊ�ò�����ʵ��
                    Class<?>[] paramTypes = methods[i].getParameterTypes();
                    if (paramTypes.length != 1)
                    {
                        returnErrorMessage("handle class's exec() method's param number is not 1!");
                        return "failed";
                    }
                    Object exec_request = objectMapper.readValue(requestBodyObj.toString(), paramTypes[0]);

                    //���exec�����ķ�������,Ҫ��̳���JsonRPCResponseBase
                    Class<?> returnType = methods[i].getReturnType();
                    String superClass = returnType.getSuperclass().getName();
                    if (superClass == null || !superClass.equals("ngse.remote_shell.JsonRPCResponseBase"))
                    {
                        returnErrorMessage("method exec() should return a class extending JsonRPCResponseBase.");
                        return "failed";
                    }

                    //����exec����,�������ص�java��ʵ�����л�Ϊjson�ַ�����Ӧ��ǰ��
                    try {
                        Object exec_result = methods[i].invoke(handler, exec_request);
                        if (exec_request != null) {
                            String s = objectMapper.writeValueAsString(exec_result);//???????bean???��??json?????
                            out.write(s.getBytes());
                        }

                        return "success";


                    } catch (InvocationTargetException e) {
                        e.printStackTrace();
                        return "failed";
                    }


                }
            }
            if (!execFound)
            {
                return "exec method not found";
            }

        } catch (ClassNotFoundException e)
        {
            e.printStackTrace();


            return "ClassNotFoundException:" + e.toString();
        } catch (InstantiationException e)
        {
            e.printStackTrace();

            return "InstantiationException:" + e.toString();
        } catch (IllegalAccessException e)
        {
            e.printStackTrace();

            return "IllegalAccessException:" + e.toString();
        }
        catch ( Exception e)
        {
            e.printStackTrace();

            return "IllegalAccessException:" + e.toString();
        }
        return "failed";

    }
    //�������ж�ȡһ��������json�ַ���
    private String readJsonString(StringBuffer jsonStr, InputStream in)
    {
        byte[] buf = new byte[10240];

        try {
            /*
            //�ȶ�һ��json�ַ����ĳ��� 10�ֽڵ��ַ���
            sum = 0;
            while (sum < 10) {
                len = in.read(buf, sum, 10-sum);
                System.out.printf("read %d bytes\n", len);
                if (len <= 0) {
                    return "read length failed!";
                }
                sum += len;
            }
            int jsonLen = new Integer(new String(buf, 0, 10).trim()).intValue();
            //��json����
            if (jsonLen > buf.length)
            {
                return "json string is too long";
            }
            sum = 0;
            while (sum < jsonLen)
            {
                len = in.read(buf, sum, buf.length-sum);
                System.out.printf("read %d bytes\n", len);
                if (len == -1)
                {
                    return "unexpected end of input";
                }
                sum += len;
            }

            jsonStr.append(new String(buf, 0, jsonLen));
            */

            //����һֱ�����Զ˹ر�д
            int offset = 0;
            while (true)
            {
                if (offset >= buf.length)
                {
                    return "buffer size is too small";
                }
                int len = in.read(buf, offset, buf.length-offset);
                if (len < 0)
                {
                    break;
                }
                offset += len;
            }
            jsonStr.append(new String(buf, 0, offset));
            return "success";
        }
        catch (IOException e)
        {
            return e.toString();
        }

    }
    @Override
    public void run() {
        try
        {
            InputStream in = socket.getInputStream();
            OutputStream out = socket.getOutputStream();
            StringBuffer jsonStr = new StringBuffer();
            String resultStr = readJsonString(jsonStr, in);
            if (!resultStr.equals("success"))
            {
                System.out.println(resultStr);

                return;
            }
            System.out.println("json string:"+jsonStr.toString());
            resultStr = handleRequest(jsonStr.toString(), in, out);
            if (!resultStr.equals("success"))
            {
                System.out.println(resultStr);
                return;
            }
            return;
        }
        catch (IOException e)
        {
            e.printStackTrace();
            return;
        }
        finally {
            System.out.println("close the socket.");
            try     {   socket.close();} catch (Exception e){}
        }

    }
}
