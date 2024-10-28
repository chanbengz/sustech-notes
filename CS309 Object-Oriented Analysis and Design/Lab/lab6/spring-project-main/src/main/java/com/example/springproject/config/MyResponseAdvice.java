package com.example.springproject.config;

import com.example.springproject.dto.GlobalResponse;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.core.MethodParameter;
import org.springframework.http.MediaType;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.servlet.mvc.method.annotation.ResponseBodyAdvice;

/**
 * Functionality Desc:
 *   全局消息封装
 *
 * @author Jiachen<zhangjc1999 @ gmail.com>
 * @date   2020/10/30 10:03 下午
 */
@ControllerAdvice(basePackages  =  "com.example.springproject.controller")
public class MyResponseAdvice implements ResponseBodyAdvice<Object> {

    @Override
    public boolean supports(MethodParameter methodParameter, Class aClass) {
        return true;
    }

    @Override
    public Object beforeBodyWrite(Object o, MethodParameter methodParameter, MediaType mediaType, Class aClass,
        ServerHttpRequest serverHttpRequest, ServerHttpResponse serverHttpResponse) {
        if (!(o instanceof GlobalResponse)) {
            GlobalResponse<Object> response = GlobalResponse.builder()
                .code(0)
                .msg("success")
                .data(o)
                .build();
            if (!(o instanceof String)) {
                return response;
            }
            ObjectMapper mapper = new ObjectMapper();
            String responseString = "";
            try {
                responseString = mapper.writeValueAsString(response);
            } catch (JsonProcessingException e) {
                e.printStackTrace();
            }
            return responseString;
        }
        return o;
    }

}