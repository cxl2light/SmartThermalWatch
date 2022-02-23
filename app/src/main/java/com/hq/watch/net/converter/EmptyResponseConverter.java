package com.hq.watch.net.converter;


import com.hq.watch.base.bean.EmptyResponse;

import java.lang.annotation.Annotation;
import java.lang.reflect.Type;

import okhttp3.ResponseBody;
import retrofit2.Converter;
import retrofit2.Retrofit;

/**
 * Created on 2021/1/1
 * author :
 * desc :
 */
public class EmptyResponseConverter extends Converter.Factory {

    private EmptyResponseConverter() {

    }

    public static EmptyResponseConverter create() {
        return new EmptyResponseConverter();
    }

    @Override
    public Converter<ResponseBody, ?> responseBodyConverter(Type type, Annotation[] annotations, Retrofit retrofit) {
        if (type == EmptyResponse.class) {
            return EmptyResponseBodyConverter.INSTANCE;
        }
        return super.responseBodyConverter(type, annotations, retrofit);
    }

    static final class EmptyResponseBodyConverter implements Converter<ResponseBody, EmptyResponse> {
        static final EmptyResponseBodyConverter INSTANCE = new EmptyResponseBodyConverter();

        @Override
        public EmptyResponse convert(ResponseBody value) {
            value.close();
            return new EmptyResponse();
        }
    }

}
