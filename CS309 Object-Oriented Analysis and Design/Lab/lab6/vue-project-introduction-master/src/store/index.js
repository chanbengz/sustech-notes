import Vue from 'vue'
import Vuex from 'vuex'

import purchase from "@/store/model/purchase"
Vue.use(Vuex)

export default new Vuex.Store({
    modules: {
        purchase
    },
    // strict: debug,
    strict: false,
})