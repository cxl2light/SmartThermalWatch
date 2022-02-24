package com.hq.watch.utils;

import io.reactivex.Observable;
import io.reactivex.subjects.PublishSubject;
import io.reactivex.subjects.Subject;

public class RxBus {
    private static volatile RxBus rxBus;
    private final Subject<Object> subject = PublishSubject.create().toSerialized();

    private RxBus(){

    }

    public static RxBus getInstance(){
        if (rxBus == null){
            synchronized (RxBus.class){
                if (rxBus == null){
                    rxBus = new RxBus();
                }
            }
        }
        return rxBus;
    }

    public void post(Object object){
        subject.onNext(object);
    }

    public <T>Observable<T> toObservable(Class<T> eventType){
        return subject.ofType(eventType);
    }
}
