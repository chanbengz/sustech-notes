import Vue from 'vue'
import App from './App.vue'

import Vuex from "vuex";
import VueRouter from "vue-router";

import ElementUI from 'element-ui'
import 'element-ui/lib/theme-chalk/index.css';

import router from '@/router'
import store from '@/store'

Vue.config.productionTip = false
Vue.use(Vuex)
Vue.use(VueRouter)
Vue.use(ElementUI)

new Vue({
    el: '#app',
    store: store,
    router,
    render: h => h(App),
    components: {
        App
    }
}).$mount('#app')
