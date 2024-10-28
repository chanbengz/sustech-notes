<template>
  <div class="container">
    <div id="main-pane">
      <el-row class="boundary" style="width: 100%; height: calc(30% - 4px); overflow: hidden">
        <div class="titletext">
          <p>OOAD Vue Introduction Project</p>
        </div>
      </el-row>

      <el-col :span="12" :offset="6">
        <el-row class="boundary" style="width: 100%; height: calc(80% - 10px); overflow: hidden; margin-top: 20px">
          <el-row>
            <p style="font-size: 30px;">Login</p>
          </el-row>
          <div id="loginForm">
            <el-col :span="20" :offset="2">
              <LoginForm :form="form"></LoginForm>
            </el-col>
          </div>
        </el-row>
      </el-col>

    </div>
  </div>
</template>

<script>
import LoginForm from "@/components/LoginForm";
import {mapState} from "vuex";

export default {
  name: "Login",
  components: {
    LoginForm
  },
  data() {
    return {}
  },
  methods: {},
  computed: {
    ...mapState("purchase", {
      form: state => state.form
    })
  }
}
</script>

<style>

.container {
  width: 100%;
  height: 100%;
}

.titletext {
  font-size: 20px;

  border-bottom-right-radius: 4px;
  border-top-left-radius: 4px;
  border-bottom-left-radius: 4px;
  border-top-right-radius: 4px;
  list-style-type: none;

  background: #dddede;
  height: 50px;
  margin: 0;
  padding: 0;
  overflow: hidden;
  color: #444444;
  text-align: center;
}

#loginForm {
  text-align: center;
  font-size: 20px;
  margin-top: 0px;
}

#main-pane {
  /*margin: 6px;*/
  /*width: calc(100% - 12px);*/
  /*height: calc(100% - 12px - 42px);*/
  margin: 6px;
  width: calc(100% - 12px);
  height: calc(100% - 12px);
  /* use grid layout rather than el-row */
  /* because el-row and el-scrollbar can't be used together */

}


</style>