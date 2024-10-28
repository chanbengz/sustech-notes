package com.example.springproject.controller;

import com.example.springproject.dto.UserDto;
import com.example.springproject.dto.UserForm;
import com.example.springproject.service.user.UserService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.validation.Valid;

@RestController
@RequestMapping(path = "/api/user")
public class UserController {

    private final UserService userService;

    @Autowired
    public UserController(UserService userService) {
        this.userService = userService;
    }

    @PostMapping("/v1/login")
    public Boolean checkLogin(@RequestBody UserDto userDto) {
        return userService.checkLogin(userDto);
    }

    @PostMapping("/v1/register")
    public UserDto registerUser(@RequestBody @Valid UserForm userForm, BindingResult result) {
        return userService.registerUser(userForm, result);
    }
}
