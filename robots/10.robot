{
   "backend" : "xl-320",
   "parameters" : {
      "BODY" : {
         "max" : "50",
         "min" : "25",
         "val" : "25"
      },
      "L1" : {
         "max" : "100",
         "min" : "45",
         "val" : "45"
      },
      "L2" : {
         "max" : "100",
         "min" : "60",
         "val" : "65"
      },
      "L3" : {
         "max" : "150",
         "min" : "30",
         "val" : "60"
      },
      "R" : {
         "max" : "15",
         "min" : "5",
         "val" : "7"
      }
   },
   "tree" : {
      "anchors" : {
         "0" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_bar",
                                 "parameters" : {
                                    "BarBottomRadius" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "BarLength" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "BarRadius" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$R"
                                    },
                                    "ScrewsCount" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "8"
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
                                       "val" : "2.3"
                                    },
                                    "UHeight" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "25"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
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
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
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
         },
         "1" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_bar",
                                 "parameters" : {
                                    "BarBottomRadius" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "BarLength" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "BarRadius" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$R"
                                    },
                                    "ScrewsCount" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "8"
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
                                       "val" : "2.3"
                                    },
                                    "UHeight" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "25"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
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
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
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
         },
         "2" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_bar",
                                 "parameters" : {
                                    "BarBottomRadius" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "BarLength" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "BarRadius" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$R"
                                    },
                                    "ScrewsCount" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "8"
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
                                       "val" : "2.3"
                                    },
                                    "UHeight" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "25"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
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
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
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
         },
         "3" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_bar",
                                 "parameters" : {
                                    "BarBottomRadius" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "BarLength" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "BarRadius" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$R"
                                    },
                                    "ScrewsCount" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "8"
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
                                       "val" : "2.3"
                                    },
                                    "UHeight" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "25"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
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
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
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
      "component" : "body_arms",
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
