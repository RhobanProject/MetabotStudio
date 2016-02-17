{
   "backend" : "xl-320",
   "parameters" : {
      "A" : {
         "max" : "45",
         "min" : "-45",
         "val" : "0"
      },
      "BODY" : {
         "max" : "100",
         "min" : "30",
         "val" : "30"
      },
      "L1" : {
         "max" : "150",
         "min" : "30",
         "val" : "50"
      },
      "L2" : {
         "max" : "150",
         "min" : "30",
         "val" : "50"
      },
      "L3" : {
         "max" : "150",
         "min" : "30",
         "val" : "60"
      }
   },
   "tips" : 4,
   "tree" : {
      "anchors" : {
         "0" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg_angle",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$A"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "15"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : -1.0471975803375244,
                              "orientationX" : 0,
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_side",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "FirstPart" : {
                              "max" : "",
                              "min" : "",
                              "val" : "17"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "double_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "90"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Screws" : {
                     "max" : "",
                     "min" : "",
                     "val" : "true"
                  },
                  "ScrewsCount" : {
                     "max" : "",
                     "min" : "",
                     "val" : "4"
                  },
                  "ScrewsDiameter" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.6"
                  },
                  "ScrewsSpacing" : {
                     "max" : "",
                     "min" : "",
                     "val" : "14"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 0,
            "zero" : 0
         },
         "1" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg_angle",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$A"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "15"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : -1.0471975803375244,
                              "orientationX" : 0,
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_side",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "FirstPart" : {
                              "max" : "",
                              "min" : "",
                              "val" : "17"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "double_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "90"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Screws" : {
                     "max" : "",
                     "min" : "",
                     "val" : "true"
                  },
                  "ScrewsCount" : {
                     "max" : "",
                     "min" : "",
                     "val" : "4"
                  },
                  "ScrewsDiameter" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.6"
                  },
                  "ScrewsSpacing" : {
                     "max" : "",
                     "min" : "",
                     "val" : "14"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 0,
            "zero" : 0
         },
         "2" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg_angle",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$A"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "15"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : -1.0471975803375244,
                              "orientationX" : 0,
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_side",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "FirstPart" : {
                              "max" : "",
                              "min" : "",
                              "val" : "17"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "double_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "90"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Screws" : {
                     "max" : "",
                     "min" : "",
                     "val" : "true"
                  },
                  "ScrewsCount" : {
                     "max" : "",
                     "min" : "",
                     "val" : "4"
                  },
                  "ScrewsDiameter" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.6"
                  },
                  "ScrewsSpacing" : {
                     "max" : "",
                     "min" : "",
                     "val" : "14"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 0,
            "zero" : 0
         },
         "3" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg_angle",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$A"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "15"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : -1.0471975803375244,
                              "orientationX" : 0,
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_side",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "FirstPart" : {
                              "max" : "",
                              "min" : "",
                              "val" : "17"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "double_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "90"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Screws" : {
                     "max" : "",
                     "min" : "",
                     "val" : "true"
                  },
                  "ScrewsCount" : {
                     "max" : "",
                     "min" : "",
                     "val" : "4"
                  },
                  "ScrewsDiameter" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.6"
                  },
                  "ScrewsSpacing" : {
                     "max" : "",
                     "min" : "",
                     "val" : "14"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 0,
            "zero" : 0
         }
      },
      "component" : "body_side",
      "parameters" : {
         "Legs" : {
            "max" : "",
            "min" : "",
            "val" : "4"
         },
         "Size" : {
            "max" : "",
            "min" : "",
            "val" : "$BODY"
         },
         "Thickness" : {
            "max" : "",
            "min" : "",
            "val" : "2.2"
         }
      }
   }
}
