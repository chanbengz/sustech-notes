<template>
  <div id="main-pane">
    <el-row class="boundary" style="width: 100%; height: calc(30% - 4px); overflow: hidden">
      <div class="titletext">
        <p>OOAD Vue Introduction Project</p>
      </div>
    </el-row>

    <el-col :span="10" :offset="6">
      <el-row class="boundary" style="width: 100%; height: calc(80% - 10px); overflow: hidden; margin-top: 20px">
        <el-row>
          <p style="font-size: 30px;">Register</p>
        </el-row>
        <div id="loginForm">
          <el-col :span="18" :offset="2">
            <RegisterForm></RegisterForm>
          </el-col>
        </div>
      </el-row>
    </el-col>

  </div>


</template>

<script>
import RegisterForm from "@/components/RegisterForm";

export default {
  name: "Register",
  components: {
    RegisterForm
  },
  data() {
    return {}
  },

  methods: {},
  computed: {}
}
</script>

<style>



</style>