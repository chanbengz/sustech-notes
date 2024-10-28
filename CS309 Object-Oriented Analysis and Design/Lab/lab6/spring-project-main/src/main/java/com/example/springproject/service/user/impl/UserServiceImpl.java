package com.example.springproject.service.user.impl;

import com.example.springproject.config.MyException;
import com.example.springproject.dto.UserDto;
import com.example.springproject.dto.UserForm;
import com.example.springproject.repository.UserRepository;
import com.example.springproject.domain.UserInfo;
import com.example.springproject.service.user.UserService;
import java.util.List;
import javax.validation.Valid;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;

@Slf4j
@Service
public class UserServiceImpl implements UserService {

    private final UserRepository userRepository;

    @Autowired
    public UserServiceImpl(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    @Override
    public UserDto save(UserInfo userInfo) {
        UserInfo user = userRepository.save(userInfo);
        return convertUser(user);
    }

    @Override
    public Boolean checkLogin(UserDto userDto) {
        if (userDto.getUsername().equals("")) {
            throw new MyException(1, "Username shouldn't be null");
        }
        if (userDto.getPassword().equals("")) {
            throw new MyException(2, "Password shouldn't be null");
        }
        UserInfo userInfo = userRepository.findUserByUsernameAndPassword(userDto.getUsername(), userDto.getPassword());
        if (userInfo == null) {
            throw new MyException(3, "Wrong username or password");
        }
        log.info("User {} login success", userDto.getUsername());
        return true;
    }

    @Override
    public UserInfo findByUsername(String username) {
        return userRepository.findUserByUsername(username);
    }

    @Override
    public UserDto registerUser(@Valid UserForm userForm, BindingResult result) {
        if (result.hasErrors()) {
            List<FieldError> errors = result.getFieldErrors();
            throw new MyException(4, errors.get(0).getDefaultMessage());
        } else if (!userForm.checkPasswordEquals()) {
            throw new MyException(5, "Two password should be same");
        } else {
            if (findByUsername(userForm.getUsername()) != null) {
                throw new MyException(6, "Duplicated user name. Please Try another");
            } else {
                return save(userForm.convertToUser());
            }

        }
    }

    private UserDto convertUser(UserInfo userInfo) {
        return UserDto.builder()
            .username(userInfo.getUsername())
            .build();
    }
}
