/*
 * This file is developed by EdgeCortix Inc. to be used with certain Renesas Electronics Hardware only.
 *
 * Copyright © 2025 EdgeCortix Inc. Licensed to Renesas Electronics Corporation with the
 * right to sublicense under the Apache License, Version 2.0.
 *
 * This file also includes source code originally developed by the Renesas Electronics Corporation.
 * The Renesas disclaimer below applies to any Renesas-originated portions for usage of the code.
 *
 * The Renesas Electronics Corporation
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED 'AS IS' AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Changed from original python code to C source code.
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 *
 * This file also includes source codes originally developed by the TensorFlow Authors which were distributed under the following conditions.
 *
 * The TensorFlow Authors
 * Copyright 2023 The Apache Software Foundation
 *
 * This product includes software developed at
 * The Apache Software Foundation (http://www.apache.org/).
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "model_io_data.h"

// Input tensor: sensor_features
// Data Type: FLOAT32
// Shape: [1, 206]
// Number of elements: 206
// Size in bytes: 824
float model_sensor_features[] = {
  0.19525385f, 0.37137842f, 0.86075735f, 1.3770628f, 0.41105342f, 1.4317822f, 0.17953253f, 1.3890069f, -0.30538082f, 0.49425459f,
  0.58357644f, -0.46247339f, -0.24965119f, -0.80986166f, 1.5670919f, -1.7731483f, 1.854651f, -0.90937495f, -0.46623397f, -0.089339733f,
  1.1668999f, 1.2486749f, 0.11557961f, -0.080091476f, 0.27217817f, -0.4288609f, 1.7023864f, 1.3443151f, -1.7158558f, -0.65041542f,
  -1.6514828f, 0.59268737f, -1.9191265f, -0.52703404f, 1.3304791f, 1.8286204f, 1.1126268f, -1.438597f, 1.4800484f, 1.4803488f,
  1.9144733f, -0.10556793f, 1.1966341f, 1.2036428f, -0.15408278f, 0.081909895f, 1.1221166f, 0.715518f, -1.5269024f, 0.88253045f,
  0.55968404f, 0.32807899f, -1.4265869f, 0.14949274f, 1.7786756f, 1.0344625f, 0.087393284f, -1.5763698f, -0.34135246f, -0.10559845f,
  -0.94177771f, -1.2546709f, 1.0969346f, 0.94767261f, -0.17539883f, -1.1337986f, 0.27373576f, -1.4591274f, -1.9248409f, -0.70343614f,
  0.47054195f, -1.4013007f, 0.44838285f, -1.1107147f, 0.46773577f, -0.4540441f, 1.7749922f, 1.6103938f, 0.72728109f, -0.20020008f,
  -0.56196856f, 0.45225382f, -0.2518723f, 1.6093943f, 0.79052472f, -1.6028788f, -1.7590983f, 1.8792362f, 0.66706681f, 0.61256003f,
  0.68255138f, -1.3163617f, -1.1584699f, -0.5673914f, -1.4842949f, 1.0027444f, -0.73828673f, 0.43132257f, -0.54515696f, -0.69981122f,
  0.28078699f, -1.8462985f, -0.24559402f, 0.53709602f, 1.9534953f, 1.8357971f, -1.591821f, 0.61116123f, -1.1644931f, 0.54023528f,
  -1.3547621f, 1.9811981f, 0.6124332f, 0.32740116f, -0.98683381f, -0.34252572f, -0.13475704f, -0.10121012f, -1.0222979f, 0.49404025f,
  -1.3641217f, -0.64796972f, -1.5584996f, 0.69900918f, 0.62531829f, -0.73119307f, -1.4472682f, 1.1133819f, -1.2136707f, 1.7982841f,
  -0.52509952f, 0.65010738f, 1.2839727f, -1.9457135f, -1.6115949f, 0.49138427f, 1.3517795f, 0.69463849f, -1.6156065f, 1.88778f,
  1.9058378f, 1.5127738f, -0.1253953f, 0.038497448f, 1.9070442f, -1.7771413f, 0.41938186f, -0.19536328f, 0.95705414f, -1.9200494f,
  -1.8432488f, -0.23315644f, -0.86877227f, 1.9183469f, -1.5192139f, -0.56222224f, -0.81543922f, -0.076426029f, -1.5250893f, 0.75464463f,
  -0.7280674f, 1.5219035f, -0.3429482f, 1.6729417f, -1.7434101f, -1.1327116f, 0.7698884f, 0.2607553f, 0.26640558f, 1.4604101f,
  -0.93844223f, 0.035875797f, 0.092992067f, 1.6668918f, -1.624238f, 1.6846304f, 0.3037858f, -1.6675501f, 1.7171848f, -0.88912582f,
  -0.72572422f, -1.9625733f, 0.66964149f, 1.3693683f, -1.4728086f, 0.58869648f, 0.86530876f, 1.3655443f, -0.84237576f, -0.94107938f,
  -1.2672346f, -0.40871716f, 0.34605169f, 0.21128583f, -1.91957f, -1.3402383f, 1.3157599f, -0.52076769f, -1.9812181f, -1.414233f,
  0.71126604f, 0.27847362f, -0.91996813f, 0.81494904f, 0.94077587f, -0.84609437f,
};

// Output tensor: cls_xi_kappa
// Data Type: FLOAT32
// Shape: [1, 6]
// Number of elements: 6
// Size in bytes: 24
float model_cls_xi_kappa[] = {
  1f, 3.3682628e-08f, 3.2401135e-10f, 0.09425731f, 0.029067894f, 35.269474f,
};

