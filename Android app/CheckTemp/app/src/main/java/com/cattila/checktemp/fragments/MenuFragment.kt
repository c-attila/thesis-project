package com.cattila.checktemp.fragments

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.databinding.DataBindingUtil
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.cattila.checktemp.R
import com.cattila.checktemp.databinding.FragmentMenuBinding

class MenuFragment : Fragment() {

    lateinit var binding: FragmentMenuBinding

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = DataBindingUtil.inflate(
            inflater, R.layout.fragment_menu, container, false)

        binding.menuButton1.setOnClickListener {
            findNavController().navigate(MenuFragmentDirections.actionMenuFragmentToCurrentTempFragment())
        }

        binding.menuButton2.setOnClickListener {
            findNavController().navigate(MenuFragmentDirections.actionMenuFragmentToLast24hTempFragment())
        }

        binding.menuButton3.setOnClickListener {
            findNavController().navigate(MenuFragmentDirections.actionMenuFragmentToLastWeekTempFragment())
        }

        return binding.root
    }
}